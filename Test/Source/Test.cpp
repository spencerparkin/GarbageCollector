#include "Test.h"
#include "GarbageCollector.h"
#include "Reference.h"

int main(int argc, char** argv)
{
	// Test #1:
	{
		GC::GarbageCollector gc;
		GC::GarbageCollector::Set(&gc);
		{
			GC::Reference<Blob, true> refA(new Blob());
			GC::Reference<Blorp, true> refB(new Blorp());
		}
	}

	// Test #2:
	{
		GC::GarbageCollector gc;
		GC::GarbageCollector::Set(&gc);
		{
			GC::Reference<Blob, true> refA(new Blob());
			GC::Reference<Blorp, true> refB(new Blorp());

			refA.Get()->ref.Set(new Blorp());
			refB.Get()->ref.Set(new Blob());
		}
	}

	// Test #3:
	{
		GC::GarbageCollector gc;
		GC::GarbageCollector::Set(&gc);
		{
			GC::Reference<Blob, true> refA(new Blob());
			GC::Reference<Blorp, true> refB(new Blorp());

			refA.Get()->ref.Set(refB.Get());
			refB.Get()->ref.Set(refA.Get());
		}
	}

	return 0;
}

//---------------------------------- Blob ----------------------------------

Blob::Blob()
{
}

/*virtual*/ Blob::~Blob()
{
}

/*virtual*/ bool Blob::IterationBegin(void*& userData)
{
	bool* iterated = new bool;
	*iterated = false;
	userData = iterated;
	return true;
}

/*virtual*/ GC::Object* Blob::IterationNext(void* userData)
{
	bool* iterated = (bool*)userData;
	if (*iterated)
		return nullptr;

	*iterated = true;
	return &this->ref;
}

/*virtual*/ void Blob::IterationEnd(void* userData)
{
	bool* iterated = (bool*)userData;
	delete iterated;
}

//---------------------------------- Blorp ----------------------------------

Blorp::Blorp()
{
}

/*virtual*/ Blorp::~Blorp()
{
}

/*virtual*/ bool Blorp::IterationBegin(void*& userData)
{
	bool* iterated = new bool;
	*iterated = false;
	userData = iterated;
	return true;
}

/*virtual*/ GC::Object* Blorp::IterationNext(void* userData)
{
	bool* iterated = (bool*)userData;
	if (*iterated)
		return nullptr;

	*iterated = true;
	return &this->ref;
}

/*virtual*/ void Blorp::IterationEnd(void* userData)
{
	bool* iterated = (bool*)userData;
	delete iterated;
}