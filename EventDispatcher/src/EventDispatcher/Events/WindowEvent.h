#pragma once
#pragma once

#include "Event.h"

class WindowEvent : public Event
{
public:

protected:
	WindowEvent() {}
};

class WindowClosedEvent : public WindowEvent
{
public:
	WindowClosedEvent(): WindowEvent() {}
	EVENT_CLASS_TYPE(WindowClosed);

protected:
};

class WindowResizedEvent : public WindowEvent
{
public:
	WindowResizedEvent(int width, int height) : WindowEvent(), m_Width(width), m_Height(height) {}
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
		return ss.str();
	}

	EVENT_CLASS_TYPE(WindowResized);
protected:
	int m_Width = -1, m_Height = -1;
};