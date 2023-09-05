#include "GarbageCollector.h"
#include "Object.h"
#include "Reference.h"
#include <assert.h>

namespace GC
{
	GarbageCollector* theGC = nullptr;
}

using namespace GC;

/*static*/ void GarbageCollector::Set(GarbageCollector* gc)
{
	theGC = gc;
}

/*static*/ GarbageCollector* GarbageCollector::Get()
{
	return theGC;
}

GarbageCollector::GarbageCollector()
{
	this->objectSet = new ObjectSet();
}

/*virtual*/ GarbageCollector::~GarbageCollector()
{
	this->Collect();
	assert(this->objectSet->size() == 0);
	delete this->objectSet;
}

void GarbageCollector::Collect()
{
	ObjectSet queue;
	for (Object* object : *this->objectSet)
		queue.insert(object);

	while (queue.size() > 0)
	{
		Object* object = *queue.begin();

		ObjectSet group;
		bool canCollect = this->FindGroup(object, group);
		assert(group.size() > 0);

		for (Object* groupMember : group)
			queue.erase(groupMember);

		if (canCollect)
		{
			for (Object* groupMember : group)
			{
				if (groupMember->GetType() == Object::Type::REF)
				{
					ReferenceBase* ref = (ReferenceBase*)groupMember;
					ref->RawClear();
				}
			}

			for (Object* groupMember : group)
			{
				if (groupMember->GetType() == Object::Type::COLLECTABLE)
				{
					Collectable* collectable = (Collectable*)groupMember;
					delete collectable;
				}
			}
		}
	}
}

bool GarbageCollector::FindGroup(Object* initialObject, ObjectSet& group)
{
	bool canCollect = true;

	ObjectSet queue;
	queue.insert(initialObject);

	while (queue.size() > 0)
	{
		Object* parentObject = *queue.begin();
		queue.erase(parentObject);

		if (parentObject->GetType() == Object::Type::REF)
		{
			ReferenceBase* ref = (ReferenceBase*)parentObject;
			if (ref->IsCritical())
				canCollect = false;
		}

		void* userData = nullptr;
		if (parentObject->IterationBegin(userData))
		{
			while (true)
			{
				Object* childObject = parentObject->IterationNext(userData);
				if (!childObject)
					break;

				queue.insert(childObject);
			}

			parentObject->IterationEnd(userData);
		}
	}

	return canCollect;
}

void GarbageCollector::Register(Object* object)
{
	assert(this->objectSet->find(object) == this->objectSet->end());
	this->objectSet->insert(object);
}

void GarbageCollector::Unregister(Object* object)
{
	assert(this->objectSet->find(object) != this->objectSet->end());
	this->objectSet->erase(object);
}