#pragma once

#include "Events/Event.h"

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
	EventQueue() = default;
	~EventQueue() = default;

	void QueueEvent(std::unique_ptr<Event> event)
	{
		assert(event != nullptr && "Cannot queue a null event pointer!");
		
		m_Buffers[m_WriteIndex].push_back(std::move(event)); // Push the queued event into the active write buffer.
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
	}
protected:
	std::array<std::vector<std::unique_ptr<Event>>, 2> m_Buffers;
	int m_WriteIndex = 0;
};