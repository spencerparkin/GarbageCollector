#pragma once

#include "Defines.h"

namespace GC
{
	class GarbageCollector;

	class GC_API Object
	{
	public:
		Object();
		virtual ~Object();

		enum class Type
		{
			REF,
			COLLECTABLE
		};

		virtual Type GetType() const = 0;
		virtual bool IterationBegin(void*& userData) = 0;
		virtual Object* IterationNext(void* userData) = 0;
		virtual void IterationEnd(void* userData) = 0;
	};
}