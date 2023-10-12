#pragma once

#include "Defines.h"

namespace GC
{
	template<typename T>
	class GC_API LinkedList
	{
	public:
		LinkedList()
		{
			this->head = nullptr;
			this->tail = nullptr;

			this->count = 0;
		}

		virtual ~LinkedList()
		{
		}

		void InsertBefore(T* before, T* item)
		{
			if (this->count == 0)
				this->Couple(item, nullptr, nullptr);
			else
				this->Couple(item, before->prev, before);

			this->count++;
		}

		void InsertAfter(T* after, T* item)
		{
			if (this->count == 0)
				this->Couple(item, nullptr, nullptr);
			else
				this->Couple(item, after, after->next);

			this->count++;
		}

		void InsertBeforeHead(T* item)
		{
			this->InsertBefore(this->head, item);
		}

		void InsertAfterTail(T* item)
		{
			this->InsertAfter(this->tail, item);
		}

		void Remove(T* item)
		{
			this->Decouple(item);

			this->count--;
		}

		int GetCount() const { return this->count; }
		T* GetHead() { return this->head; }
		T* GetTail() { return this->tail; }

	private:

		void Decouple(T* item)
		{
			if (item->prev)
				item->prev->next = item->next;
			else
				this->head = this->head->next;

			if (item->next)
				item->next->prev = item->prev;
			else
				this->tail = this->tail->prev;

			item->next = nullptr;
			item->prev = nullptr;
		}

		void Couple(T* item, T* prev, T* next)
		{
			item->prev = prev;
			item->next = next;
			
			if (prev)
				prev->next = item;
			else
				this->head = item;

			if (next)
				next->prev = item;
			else
				this->tail = item;
		}

		T* head;
		T* tail;

		int count;
	};
}