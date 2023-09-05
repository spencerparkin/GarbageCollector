#pragma once

#include "Collectable.h"

namespace GC
{
	class ReferenceBase : public Object
	{
	public:
		ReferenceBase(GarbageCollector* gc, bool critical) : Object(gc)
		{
			this->critical = critical;
		}

		virtual ~ReferenceBase()
		{
		}

		virtual Type GetType() const override
		{
			return Type::REF;
		}

		bool IsCritical() const
		{
			return this->critical;
		}

	private:
		bool critical;
	};

	// The memory for instances of this class is managed by the application and typically allocated on the stack or owned by a collectable.
	// All those of the latter case should be non-critical references.  Collectables not connected to a critical reference, directly or indirectly,
	// are collected by the GC.  Most deallocations are expected to happen here, however, when reference counts go to zero.  When a cycle is
	// created in the reference graph, however, then there may become a need for the GC to detect whether a collectable can be freed.
	template<typename CollectableDerivative, bool critical>
	class Reference : public ReferenceBase
	{
	public:
		Reference(GarbageCollector* gc) : ReferenceBase(gc, critical)
		{
			this->collectable = nullptr;
		}

		Reference(GarbageCollector* gc, CollectableDerivative* collectable) : ReferenceBase(gc, critical)
		{
			this->collectable = nullptr;
			this->Set(collectable);
		}

		void Set(CollectableDerivative* collectable)
		{
			if (this->collectable)
				if (--this->collectable->refCount == 0)
					delete this->collectable;

			this->collectable = collectable;

			if (this->collectable)
				this->collectable->refCount++;
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
			userData = nullptr;
			return this->collectable != nullptr;
		}

		virtual Object* IterationNext(void* userData) override
		{
			return this->collectable;
		}

		virtual void IterationEnd(void* userData) override
		{
		}

	private:
		CollectableDerivative* collectable;
	};
}