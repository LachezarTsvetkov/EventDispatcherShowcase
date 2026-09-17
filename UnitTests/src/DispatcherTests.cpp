#include <gtest/gtest.h>
#include "EventDispatcher/Dispatcher.h"
#include "EventDispatcher/Events/WindowEvent.h"
#include "EventDispatcher/Events/MouseEvent.h"

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

class NestedEvents : public Event
{
public:

protected:
	NestedEvents() {}
	EVENT_CLASS_TYPE(Custom);
};

class DispatcherTestFixture : public testing::Test
{
protected:
	bool windowClosedCalled = false;
	bool mouseMovedCalled = false;

	bool OnWindowClosed(WindowClosedEvent& e)
	{
		windowClosedCalled = true;
		return true;
	}

	bool OnMouseMoved(MouseMovedEvent& e)
	{
		mouseMovedCalled = true;
		return false;
	}

	void RouteEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.DispatchEvent<WindowClosedEvent>(BIND_EVENT_FN(DispatcherTestFixture::OnWindowClosed));
		dispatcher.DispatchEvent<MouseMovedEvent>(BIND_EVENT_FN(DispatcherTestFixture::OnMouseMoved));
	}
};

TEST_F(DispatcherTestFixture, TypeMismatchRejection)
{
	MouseMovedEvent mouseEvent(0, 0);

	RouteEvent(mouseEvent);

	EXPECT_FALSE(windowClosedCalled);
	EXPECT_TRUE(mouseMovedCalled);
}

TEST_F(DispatcherTestFixture, HandledFlagPropagation)
{
	WindowClosedEvent closeEvent;
	RouteEvent(closeEvent);

	// Assert the listener executed and successfully altered the event's internal state
	EXPECT_TRUE(windowClosedCalled);
	EXPECT_TRUE(closeEvent.Handled);
}