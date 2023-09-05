#include "Object.h"
#include "GarbageCollector.h"

using namespace GC;

Object::Object(GarbageCollector* gc)
{
	this->gc = gc;
	this->gc->Register(this);
}

/*virtual*/ Object::~Object()
{
	this->gc->Unregister(this);
}