#include "GarbageCollector.h"
#include "Object.h"
#include "Reference.h"
#include <vector>
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
	this->visitationKey = 0;
	this->objectSet = new ObjectSet();
}

/*virtual*/ GarbageCollector::~GarbageCollector()
{
	this->Collect();
	assert(this->objectSet->size() == 0);
	delete this->objectSet;
}

void GarbageCollector::CreateGraph(GraphMap& graphMap)
{
	for (Object* objectA : *this->objectSet)
	{
		void* userData = nullptr;
		if (objectA->IterationBegin(userData))
		{
			while (true)
			{
				Object* objectB = objectA->IterationNext(userData);
				if (!objectB)
					break;

				this->CreateEdge(graphMap, objectA, objectB);
				this->CreateEdge(graphMap, objectB, objectA);
			}

			objectA->IterationEnd(userData);
		}
	}
}

void GarbageCollector::CreateEdge(GraphMap& graphMap, Object* objectA, Object* objectB)
{
	std::vector<Object*>* adjacentObjectArray = nullptr;
	GraphMap::iterator iter = graphMap.find(objectA);
	if (iter != graphMap.end())
		adjacentObjectArray = iter->second;
	else
	{
		adjacentObjectArray = new std::vector<Object*>();
		graphMap.insert(std::pair<Object*, std::vector<Object*>*>(objectA, adjacentObjectArray));
	}

	adjacentObjectArray->push_back(objectB);
}

void GarbageCollector::DestroyGraph(GraphMap& graphMap)
{
	for (std::pair<Object*, std::vector<Object*>*> pair : graphMap)
		delete pair.second;

	graphMap.clear();
}

void GarbageCollector::Collect()
{
	GraphMap graphMap;
	this->CreateGraph(graphMap);

	ObjectSet queue;
	for (Object* object : *this->objectSet)
		queue.insert(object);

	this->visitationKey++;

	while (queue.size() > 0)
	{
		Object* object = *queue.begin();

		ObjectSet group;
		bool canCollect = this->FindGroup(object, group, graphMap);
		assert(group.size() > 0);

		for (Object* groupMember : group)
			queue.erase(groupMember);

		if (canCollect)
		{
			std::vector<Collectable*> doomedCollectableArray;

			for (Object* groupMember : group)
			{
				switch (groupMember->GetType())
				{
					case Object::Type::REF:
					{
						ReferenceBase* ref = (ReferenceBase*)groupMember;
						ref->RawClear();
						break;
					}
					case Object::Type::COLLECTABLE:
					{
						doomedCollectableArray.push_back((Collectable*)groupMember);
						break;
					}
				}
			}

			for (Collectable* collectable : doomedCollectableArray)
				delete collectable;
		}
	}

	this->DestroyGraph(graphMap);
}

bool GarbageCollector::FindGroup(Object* initialObject, ObjectSet& group, GraphMap& graphMap)
{
	bool canCollect = true;

	ObjectSet queue;
	queue.insert(initialObject);

	while (queue.size() > 0)
	{
		Object* object = *queue.begin();
		queue.erase(object);
		group.insert(object);
		object->visitationKey = this->visitationKey;

		if (object->GetType() == Object::Type::REF)
		{
			ReferenceBase* ref = (ReferenceBase*)object;
			if (ref->IsCritical())
				canCollect = false;
		}

		GraphMap::iterator iter = graphMap.find(object);
		if (iter != graphMap.end())
			for (Object* adjacentObject : *iter->second)
				if (adjacentObject->visitationKey != this->visitationKey && queue.find(adjacentObject) == queue.end())
					queue.insert(adjacentObject);
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