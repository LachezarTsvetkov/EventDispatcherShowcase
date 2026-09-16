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
	~EventQueue() {
		for each(Event* event in eventList)
		{
			delete event;
		}
	}

	void QueueEvent(Event* event)
	{
		eventList.push_back(event);
	}

	void ExecuteQueuedEvents(std::function<void(Event&)> eventHandler)
	{
		for each(Event* event in eventList)
		{
			eventHandler(*event);

			delete event;
		}
		eventList.clear();
	}
protected:
	std::vector<Event*> eventList;
};