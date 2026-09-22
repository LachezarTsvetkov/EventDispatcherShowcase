#pragma once
#include "EventDispatcher/Events/Event.h"
#include "EventDispatcher/Dispatcher.h"
#include "EventDispatcher/Events/WindowEvent.h"
#include "EventDispatcher/Events/MouseEvent.h"
#include "EventDispatcher/Events/KeyEvent.h"

class Application
{
public:
	Application();
	~Application();

	void Run();
	void OnEvent(Event&);

private:
	bool isRunning = false;
	bool OnWindowClosed(WindowClosedEvent&);
	bool OnWindowResized(WindowResizedEvent&);
	
	bool OnMouseMoved(MouseMovedEvent&);
	bool OnMouseClicked(MouseButtonClickedEvent&);
	bool OnMousePressed(MouseButtonPressedEvent&);
	bool OnMouseReleased(MouseButtonReleasedEvent&);
	bool OnMouseScrolled(MouseScrolledEvent&);

	bool OnKeyPressed(KeyPressedEvent&);
	bool OnKeyReleased(KeyReleasedEvent&);

	EventQueue eventQueue;
};

