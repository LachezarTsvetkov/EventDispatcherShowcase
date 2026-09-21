#pragma once

#include "Events/Event.h"
#include "Memory/EventAllocator.h"

class EventDispatcher
{
	template<typename T>
	using EventFn = std::function<bool(T&)>;
public:
	EventDispatcher(Event& event) : m_Event(event) {}

	template<typename T>
	bool DispatchEvent(EventFn<T> func)
	{
		assert(func && "Cannot dispatch to an empty std::function!");
		if (m_Event.Handled) return false;

		if (m_Event.GetEventType() == T::GetStaticType())
		{
			m_Event.Handled |= func(static_cast<T&>(m_Event));
			return true;
		}
		return false;
	}
protected:
	Event& m_Event;
};

class EventQueue
{
public:
	EventQueue(size_t bufferSizeBytes = 2 * 1024 * 1024)
		: m_Allocators{ LinearAllocator(bufferSizeBytes), LinearAllocator(bufferSizeBytes) }
	{}

	~EventQueue() = default;

	template<typename T, typename... Args>
	void QueueEvent(Args&&... args)
	{
		LinearAllocator& writeAllocator = m_Allocators[m_WriteIndex];

		// Construct the event in the active write buffer using the custom allocator;l
		EventPtr event = EventFactory::Create<T>(writeAllocator, std::forward<Args>(args)...);
		assert(event != nullptr && "Event Queue allocator out of memory!");


		m_Buffers[m_WriteIndex].push_back(std::move(event));// Push the queued event into the active write buffer.
	}

	void ExecuteQueuedEvents(std::function<void(Event&)> eventHandler)
	{
		int readIndex = m_WriteIndex;
		// O(1) memory swap, where we switch the write buffer to the other index in an extremely performant manner.
		m_WriteIndex = (m_WriteIndex + 1) % 2;

		for (auto& event : m_Buffers[readIndex])
		{
			eventHandler(*event);
		}

		// Ensure the read buffer is cleared after processing to avoid memory leaks and prepare for the next cycle.
		m_Buffers[readIndex].clear();
		m_Allocators[readIndex].Reset();
	}
protected:
	std::array<std::vector<EventPtr>, 2> m_Buffers;
	std::array<LinearAllocator, 2> m_Allocators;
	int m_WriteIndex = 0;
};