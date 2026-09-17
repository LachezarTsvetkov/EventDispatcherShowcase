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
	~EventQueue() = default;

	void QueueEvent(std::unique_ptr<Event> event)
	{
		assert(event != nullptr && "Cannot queue a null event pointer!");
		eventList.push_back(std::move(event));
	}

	void ExecuteQueuedEvents(std::function<void(Event&)> eventHandler)
	{
		std::vector<std::unique_ptr<Event>> currentFrameEvents;
		currentFrameEvents.swap(eventList);

		for (auto& event : currentFrameEvents)
		{
			eventHandler(*event);
		}
		//currentFrameEvents.clear(); //Not needed, as the object's lifespan ends here
	}
protected:
	std::vector<std::unique_ptr<Event>> eventList;
};