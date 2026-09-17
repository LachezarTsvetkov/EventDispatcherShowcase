#include <gtest/gtest.h>
#include "EventDispatcher/Dispatcher.h"
#include "EventDispatcher/Events/WindowEvent.h"
#include "EventDispatcher/Events/MouseEvent.h"
#include "EventDispatcher/Events/KeyEvent.h"

class EventQueueTestFixture : public testing::Test
{
protected:
	EventQueue queue;
	std::vector<EventType> executionOrder;

	// Mocked callback to log the order of the events as they are processed
	void RouteQueuedEvent(Event& e)
	{
		executionOrder.push_back(e.GetEventType());
	}
};

TEST_F(EventQueueTestFixture, FIFOExecutionOrder)
{
	queue.QueueEvent(new MouseMovedEvent(10, 10));
	queue.QueueEvent(new KeyPressedEvent(65));
	queue.QueueEvent(new WindowClosedEvent());

	queue.ExecuteQueuedEvents([this](Event& e) { RouteQueuedEvent(e); });

	ASSERT_EQ(executionOrder.size(), 3);
	EXPECT_EQ(executionOrder[0], EventType::MouseMoved);
	EXPECT_EQ(executionOrder[1], EventType::KeyPressed);
	EXPECT_EQ(executionOrder[2], EventType::WindowClosed);
}

TEST_F(EventQueueTestFixture, EmptyQueueSafety)
{
	// Invoking `ExecuteQueuedEvents` with an empty queue should not lead to problems
	queue.ExecuteQueuedEvents([this](Event& e) { RouteQueuedEvent(e); });

	EXPECT_TRUE(executionOrder.empty());
}

TEST_F(EventQueueTestFixture, SyncVsDeferredExecutionOrder)
{
	queue.QueueEvent(new MouseMovedEvent(10, 10));

	WindowClosedEvent syncEvent;
	RouteQueuedEvent(syncEvent);

	queue.ExecuteQueuedEvents([this](Event& e) { RouteQueuedEvent(e); });

	ASSERT_EQ(executionOrder.size(), 2);
	EXPECT_EQ(executionOrder[0], EventType::WindowClosed);
	EXPECT_EQ(executionOrder[1], EventType::MouseMoved);
}

using EventQueueDeathTest = EventQueueTestFixture;

TEST_F(EventQueueDeathTest, NullPointerRejection)
{
	EXPECT_DEATH({
		queue.QueueEvent(nullptr);
		}, "Cannot queue a null event pointer!");
}