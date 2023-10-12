#pragma once

#include "Defines.h"
#include "LinkedList.h"
#include <set>
#include <map>
#include <vector>

namespace GC
{
	class Object;

	class GC_API GarbageCollector
	{
		friend Object;

	public:
		GarbageCollector();
		virtual ~GarbageCollector();

		static void Set(GarbageCollector* gc);
		static GarbageCollector* Get();

		// A single call to this should detect and free all collectables that can be collected.
		// TODO: Maybe add a maximum running time argument?  If infinite is given, work as currently designed.
		//       If a run-time cap is given, then maybe a random starting point is a good stratagy?
		void Collect();

		unsigned int NumTrackedObjects() { return this->objectList.GetCount(); }

	private:
		
		void Register(Object* object);
		void Unregister(Object* object);

		typedef std::set<Object*> ObjectSet;
		typedef std::map<Object*, std::vector<Object*>*> GraphMap;

		void CreateGraph(GraphMap& graphMap);
		void DestroyGraph(GraphMap& graphMap);
		void CreateEdge(GraphMap& graphMap, Object* objectA, Object* objectB);

		bool FindGroup(Object* initialObject, ObjectSet& group, GraphMap& graphMap);
		
		int visitationKey;

		LinkedList<Object> objectList;
	};
}