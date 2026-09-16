#pragma once

#include "Event.h"

class KeyEvent : public Event
{
public:
	int GetKeyCode() { return m_keyCode;  }

protected:

	KeyEvent(int keycode) :m_keyCode(keycode) {}
	int m_keyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(int keycode) : KeyEvent(keycode) {}
	int GetRepeatCount() { return repeatCount; }

	EVENT_CLASS_TYPE(KeyPressed);

protected:
	int repeatCount = 0;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}
	
	EVENT_CLASS_TYPE(KeyReleased);
};