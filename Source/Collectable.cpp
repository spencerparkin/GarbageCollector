#include "Collectable.h"
#include "GarbageCollector.h"

using namespace GC;

Collectable::Collectable()
{
	this->refCount = 0;
}

/*virtual*/ Collectable::~Collectable()
{
}

/*virtual*/ Object::Type Collectable::GetType() const
{
	return Type::COLLECTABLE;
}

/*virtual*/ bool Collectable::IterationBegin(void*& userData)
{
	TypicalIterationData* data = new TypicalIterationData;
	data->i = 0;
	userData = data;
	this->PopulateIterationArray(data->objectArray);
	return true;
}

/*virtual*/ GC::Object* Collectable::IterationNext(void* userData)
{
	TypicalIterationData* data = (TypicalIterationData*)userData;
	if (data->i >= (signed)data->objectArray.size())
		return nullptr;

	return data->objectArray[data->i++];
}

/*virtual*/ void Collectable::IterationEnd(void* userData)
{
	TypicalIterationData* data = (TypicalIterationData*)userData;
	delete data;
}

/*virtual*/ void Collectable::PopulateIterationArray(std::vector<Object*>& iterationArray)
{
}