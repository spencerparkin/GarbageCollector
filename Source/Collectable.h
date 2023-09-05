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
		Collectable(GarbageCollector* gc);
		virtual ~Collectable();

	private:
		int refCount;
	};
}