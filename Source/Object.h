#pragma once

#include "Defines.h"
#include "LinkedList.h"
#include <vector>

namespace GC
{
	class GarbageCollector;

	class GC_API Object
	{
		friend GarbageCollector;
		friend LinkedList<Object>;

	public:
		Object();
		virtual ~Object();

		enum class Type
		{
			REF,
			COLLECTABLE
		};

		struct TypicalIterationData
		{
			std::vector<GC::Object*> objectArray;
			int i;
		};

		virtual Type GetType() const = 0;
		virtual bool IterationBegin(void*& userData) = 0;
		virtual Object* IterationNext(void* userData) = 0;
		virtual void IterationEnd(void* userData) = 0;

	private:

		Object* next;
		Object* prev;

		int visitationKey;
	};
}