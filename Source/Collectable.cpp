#include "Collectable.h"
#include "GarbageCollector.h"

using namespace GC;

Collectable::Collectable(GarbageCollector* gc) : Object(gc)
{
	this->refCount = 0;
}

/*virtual*/ Collectable::~Collectable()
{
}