#pragma once

#include "LinearAllocator.h"
#include "../Events/Event.h"
#include <memory>
#include <utility>

// Custom deleter for Event objects that ensures the destructor is called without freeing the memory.
struct EventDeleter
{
	void operator()(Event* event) const
	{
		if (event)
		{
			// Ensure the object cleans up its own internal resources.
			event->~Event();
		}
	}
};

// Alias for the custom smart pointer that uses the EventDeleter to manage Event objects.
using EventPtr = std::unique_ptr<Event, EventDeleter>;



class EventFactory
{
public:
	template<typename T, typename... Args>
	static EventPtr Create(LinearAllocator& allocator, Args&&... args) // Args&& allows this to accept any number of arguments of any type, and then forward them to the constructor of T.
	{
		// Request aligned memory from the custom arena
		// (e.g translation into code) void* memory = allocator.Allocate(sizeof(MouseMovedEvent), alignof(MouseMovedEvent))
		void* memory = allocator.Allocate(sizeof(T), alignof(T));

		if (!memory) return nullptr;

		// Construct the object directly at the 'memory' address
		// (e.g. translation into code) MouseMovedEvent* event = new (memory) MouseMovedEvent(150, 250);
		T* event = new (memory) T(std::forward<Args>(args)...);

		// Wrap the raw pointer in the custom unique_ptr and return it
		return EventPtr(event);
	}
};