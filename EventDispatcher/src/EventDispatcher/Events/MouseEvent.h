#pragma once

#include "Event.h"

class MouseEvent : public Event
{
public:
	int GetPosX() { return posX; }
	int GetPosY() { return posY; }

protected:
	MouseEvent(int posX, int posY) :posX(posX), posY(posY) {}

	int posX, posY;
};

class MouseButtonPressedEvent : public MouseEvent
{
public:
	MouseButtonPressedEvent(int posX, int posY, int buttonID) : MouseEvent(posX, posY), buttonID(buttonID) {}
	int GetRepeatCount() { return repeatCount; }
	int GetButtonID() { return buttonID; }

	EVENT_CLASS_TYPE(MouseButtonPressed);
protected:
	int buttonID = -1, repeatCount = 0;
};

class MouseButtonReleasedEvent : public MouseEvent
{
public:
	MouseButtonReleasedEvent(int posX, int posY, int buttonID) : MouseEvent(posX, posY), buttonID(buttonID) {}
	int GetButtonID() { return buttonID; }

	EVENT_CLASS_TYPE(MouseButtonReleased);
protected:
	int buttonID = -1;
};

class MouseButtonClickedEvent : public MouseEvent
{
public:
	MouseButtonClickedEvent(int posX, int posY, int buttonID) : MouseEvent(posX, posY), buttonID(buttonID) {}
	int GetButtonID() { return buttonID; }

	EVENT_CLASS_TYPE(MouseButtonClicked);
protected:
	int buttonID = -1;
};

class MouseMovedEvent : public MouseEvent
{
public:
	MouseMovedEvent(int posX, int posY) : MouseEvent(posX, posY) {}

	EVENT_CLASS_TYPE(MouseMoved);
};

class MouseScrolledEvent : public MouseEvent
{
public:
	MouseScrolledEvent(int posX, int posY, bool scrollUp) : MouseEvent(posX, posY), scrollUp(scrollUp) {}
	bool GetIsScrollUp() { return scrollUp; }

	EVENT_CLASS_TYPE(MouseScrolled);
protected:
	bool scrollUp = false;
};