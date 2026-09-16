#include "edpch.h"
#include "Application.h"

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1) //We need to use `std::bind` so the Event APIs later know exactly to which object the `this` pointer points to (aka the Application instance)

Application::Application() 
{
	isRunning = true;
}

Application::~Application() {

}

void Application::Run()
{
	unsigned int frameCnt = 0;
	while (isRunning)
	{
		frameCnt++;
		if (frameCnt == 3)
		{
			MouseMovedEvent e(100, 200);
			OnEvent(e);

			std::cout << "Successfully send a MouseMovedEvent" << std::endl;
		}
		if (frameCnt == 6)
		{
			WindowClosedEvent e;
			OnEvent(e);

			std::cout << "Successfully send a WindowClosedEvent" << std::endl;
		}
	}

}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);

	// We bind the function address, the 'this' instance, and a placeholder for the event argument
	dispatcher.DispatchEvent<WindowClosedEvent>(BIND_EVENT_FN(Application::OnWindowClosed));
}

bool Application::OnWindowClosed(WindowClosedEvent& e)
{
	isRunning = false;

	return true;
}


int main(int argc, char** argv)
{
	Application app;
	app.Run();
}