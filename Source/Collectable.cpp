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