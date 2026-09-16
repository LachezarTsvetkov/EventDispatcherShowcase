#pragma once

#include "edpch.h"

enum class EventType
{
	None,
	MouseButtonPressed, MouseButtonReleased, MouseButtonClicked, MouseMoved, MouseScrolled,
	KeyPressed, KeyReleased, 
	WindowClosed, WindowResized, 
};

#define EVENT_CLASS_TYPE(type)\
	static EventType GetStaticType() { return EventType::type; }\
	virtual EventType GetEventType() const override { return GetStaticType(); }\
	const char* GetName() const override { return #type; }

class Event
{
public:
	bool Handled = false;
	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual std::string ToString() const { return GetName(); } // While using `std::string` can result in the allocation for memory, the 'ToString()' method is only used for debugging purposes, and should not be invoked in regular runs.

	virtual ~Event() = default;
protected:
	Event() = default;
	Event& operator=(Event&);
	Event(const Event&);
};