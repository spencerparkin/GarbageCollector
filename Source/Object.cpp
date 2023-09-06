#include "Object.h"
#include "GarbageCollector.h"

using namespace GC;

Object::Object()
{
	this->visitationKey = 0;

	GarbageCollector::Get()->Register(this);
}

/*virtual*/ Object::~Object()
{
	GarbageCollector::Get()->Unregister(this);
}