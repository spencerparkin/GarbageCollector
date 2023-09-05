#pragma once

#include "Defines.h"
#include <set>

namespace GC
{
	class Object;

	class GC_API GarbageCollector
	{
		friend Object;

	public:
		GarbageCollector();
		virtual ~GarbageCollector();

		// A single call to this should detect and free all collectables that can be collected.
		// TODO: Maybe add a maximum running time argument?  If infinite is given, work as currently designed.
		//       If a run-time cap is given, then maybe a random starting point is a good stratagy?
		void Collect();

	private:
		
		void Register(Object* object);
		void Unregister(Object* object);

		typedef std::set<Object*> ObjectSet;

		bool FindGroup(Object* initialObject, ObjectSet& group);
		
		ObjectSet* objectSet;
	};
}