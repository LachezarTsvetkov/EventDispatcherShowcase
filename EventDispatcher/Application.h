#pragma once
#include "src/EventDispatcher/Events/Event.h"
#include "src/EventDispatcher/Dispatcher.h"
#include "src/EventDispatcher/Events/WindowEvent.h"
#include "src/EventDispatcher/Events/MouseEvent.h"

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
};

