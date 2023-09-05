#include "Object.h"
#include "GarbageCollector.h"

using namespace GC;

Object::Object()
{
	GarbageCollector::Get()->Register(this);
}

/*virtual*/ Object::~Object()
{
	GarbageCollector::Get()->Unregister(this);
}