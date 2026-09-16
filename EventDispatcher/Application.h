#pragma once
#include "src/EventDispatcher/Events/Event.h"
#include "src/EventDispatcher/Dispatcher.h"
#include "src/EventDispatcher/Events/WindowEvent.h"
#include "src/EventDispatcher/Events/MouseEvent.h"
#include "src/EventDispatcher/Events/KeyEvent.h"

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

