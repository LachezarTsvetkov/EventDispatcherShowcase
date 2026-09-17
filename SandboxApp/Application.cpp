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
		if (frameCnt == 10)
		{
			std::cout << "Sending synchronous events to the Application instance" << std::endl;
			MouseMovedEvent e(100, 200);
			OnEvent(e);

			KeyPressedEvent e2(65);
			OnEvent(e2);
		}
		if (frameCnt == 20)
		{
			std::cout << "Sending synchronous and deferred events to the Application instance" << std::endl;
			eventQueue.QueueEvent(std::make_unique<MouseScrolledEvent>(10, 20, true));

			MouseMovedEvent e(300, 400);
			OnEvent(e);

			MouseButtonClickedEvent e2 (1, 2, 3);
			OnEvent(e2);

			eventQueue.QueueEvent(std::make_unique<WindowClosedEvent>());
		}

		eventQueue.ExecuteQueuedEvents([this](Event& e) {
			OnEvent(e);
		});
	}

}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);

	// We bind the function address, the 'this' instance, and a placeholder for the event argument
	dispatcher.DispatchEvent<WindowClosedEvent>(BIND_EVENT_FN(Application::OnWindowClosed));
	dispatcher.DispatchEvent<WindowResizedEvent>(BIND_EVENT_FN(Application::OnWindowResized));
	dispatcher.DispatchEvent<MouseMovedEvent>(BIND_EVENT_FN(Application::OnMouseMoved));
	dispatcher.DispatchEvent<MouseButtonClickedEvent>(BIND_EVENT_FN(Application::OnMouseClicked));
	dispatcher.DispatchEvent<MouseButtonPressedEvent>(BIND_EVENT_FN(Application::OnMousePressed));
	dispatcher.DispatchEvent<MouseButtonReleasedEvent>(BIND_EVENT_FN(Application::OnMouseReleased));
	dispatcher.DispatchEvent<MouseScrolledEvent>(BIND_EVENT_FN(Application::OnMouseScrolled));
	dispatcher.DispatchEvent<KeyPressedEvent>(BIND_EVENT_FN(Application::OnKeyPressed));
	dispatcher.DispatchEvent<KeyReleasedEvent>(BIND_EVENT_FN(Application::OnKeyReleased));
}

bool Application::OnWindowClosed(WindowClosedEvent& e)
{
	isRunning = false;
	std::cout << "Window closed event received. Stopping the application." << std::endl;

	return true;
}

bool Application::OnWindowResized(WindowResizedEvent& e)
{
	std::cout << "Window resized to: " << e.GetWidth() << "x" << e.GetHeight() << std::endl;
	return true;
}

bool Application::OnMouseMoved(MouseMovedEvent& e)
{
	std::cout << "Mouse moved to: " << e.GetPosX() << ", " << e.GetPosY() << std::endl;
	return true;
}

bool Application::OnMouseClicked(MouseButtonClickedEvent& e)
{
	std::cout << "Mouse button clicked: " << e.GetButtonID() << std::endl;
	return true;
}

bool Application::OnMousePressed(MouseButtonPressedEvent& e)
{
	std::cout << "Mouse button pressed: " << e.GetButtonID() << std::endl;
	return true;
}

bool Application::OnMouseReleased(MouseButtonReleasedEvent& e)
{
	std::cout << "Mouse button released: " << e.GetButtonID() << std::endl;
	return true;
}

bool Application::OnMouseScrolled(MouseScrolledEvent& e)
{
	std::cout << "Mouse scrolled at: " << e.GetPosX() << ", " << e.GetPosY() << " Scroll Up: " << (e.GetIsScrollUp() ? "Yes" : "No") << std::endl;
	return true;
}

bool Application::OnKeyPressed(KeyPressedEvent& e)
{
	std::cout << "Key pressed: " << e.GetKeyCode() << std::endl;
	return true;
}

bool Application::OnKeyReleased(KeyReleasedEvent& e)
{
	std::cout << "Key released: " << e.GetKeyCode() << std::endl;
	return true;
}

int main(int argc, char** argv)
{
	Application app;
	app.Run();
}