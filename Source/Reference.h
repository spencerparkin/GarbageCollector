#pragma once

#include "Collectable.h"

namespace GC
{
	class GC_API ReferenceBase : public Object
	{
	public:
		ReferenceBase(bool isCritical)
		{
			this->isCritical = isCritical;
		}

		virtual ~ReferenceBase()
		{
		}

		virtual Type GetType() const override
		{
			return Type::REF;
		}

		virtual void RawClear() = 0;

		bool IsCritical() const
		{
			return this->isCritical;
		}

	private:
		bool isCritical;
	};

	// The memory for instances of this class is managed by the application and typically allocated on the stack or owned by a collectable.
	// All those of the latter case should be non-critical references.  Collectables not connected to a critical reference, directly or indirectly,
	// are collected by the GC.  Most deallocations are expected to happen here, however, when reference counts go to zero.  When a cycle is
	// created in the reference graph, however, then there may become a need for the GC to detect whether a collectable can be freed.
	template<typename CollectableDerivative, bool critical>
	class GC_API Reference : public ReferenceBase
	{
	public:
		Reference() : ReferenceBase(critical)
		{
			this->collectable = nullptr;
		}

		Reference(CollectableDerivative* collectable) : ReferenceBase(critical)
		{
			this->collectable = nullptr;
			this->Set(collectable);
		}

		virtual ~Reference()
		{
			this->Set(nullptr);
		}

		virtual void RawClear() override
		{
			this->collectable = nullptr;
		}

		void Set(CollectableDerivative* collectable)
		{
			if (this->collectable)
				if (--((Collectable*)this->collectable)->refCount == 0)
					delete this->collectable;

			this->collectable = collectable;

			if (this->collectable)
				((Collectable*)this->collectable)->refCount++;
		}

		CollectableDerivative* Get()
		{
			return this->collectable;
		}

		const CollectableDerivative* Get() const
		{
			return this->collectable;
		}

		virtual bool IterationBegin(void*& userData) override
		{
			if (!this->collectable)
				return false;

			bool* iterated = new bool;
			*iterated = false;
			userData = iterated;
			return true;
		}

		virtual Object* IterationNext(void* userData) override
		{
			bool* iterated = (bool*)userData;
			if (*iterated)
				return nullptr;

			*iterated = true;
			return (Object*)this->collectable;
		}

		virtual void IterationEnd(void* userData) override
		{
			bool* iterated = (bool*)userData;
			delete iterated;
		}

	private:
		CollectableDerivative* collectable;
	};
}