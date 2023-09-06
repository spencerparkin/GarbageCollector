#pragma once

#include "Object.h"

namespace GC
{
	// It is the memory for derivatives of this class that is managed by the GC.
	class GC_API Collectable : public Object
	{
		template<typename CollectableDerivative, bool critical>
		friend class Reference;

	public:
		Collectable();
		virtual ~Collectable();

		virtual Type GetType() const override;
		virtual bool IterationBegin(void*& userData) override;
		virtual Object* IterationNext(void* userData) override;
		virtual void IterationEnd(void* userData) override;

		virtual void PopulateIterationArray(std::vector<Object*>& iterationArray);

	private:
		int refCount;
	};
}