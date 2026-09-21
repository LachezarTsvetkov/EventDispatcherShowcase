#include <gtest/gtest.h>
#include "EventDispatcher/Dispatcher.h"
#include "EventDispatcher/Events/WindowEvent.h"
#include "EventDispatcher/Events/MouseEvent.h"

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

class AdvancedDispatcherTest : public testing::Test
{
public:
	int callCount = 0;

	bool HandlerOne(WindowClosedEvent& e) {
		callCount++;
		return true;
	}

	bool HandlerTwo(WindowClosedEvent& e) {
		callCount++;
		return true;
	}
};

TEST_F(AdvancedDispatcherTest, EventGhostingPrevention)
{
	WindowClosedEvent e;
	EventDispatcher dispatcher(e);

	dispatcher.DispatchEvent<WindowClosedEvent>(BIND_EVENT_FN(AdvancedDispatcherTest::HandlerOne));
	dispatcher.DispatchEvent<WindowClosedEvent>(BIND_EVENT_FN(AdvancedDispatcherTest::HandlerTwo));

	EXPECT_EQ(callCount, 1);
	EXPECT_TRUE(e.Handled);
}

using DispatcherDeathTest = AdvancedDispatcherTest;

TEST_F(DispatcherDeathTest, EmptyFunctionRejection)
{
	WindowClosedEvent e;
	EventDispatcher dispatcher(e);

	std::function<bool(WindowClosedEvent&)> emptyFunc = nullptr;

	EXPECT_DEATH({
		dispatcher.DispatchEvent<WindowClosedEvent>(emptyFunc);
		}, "Cannot dispatch to an empty std::function!");
}



class AdvancedQueueTest : public testing::Test
{
protected:
	EventQueue queue;
	int processedEvents = 0;
};

TEST_F(AdvancedQueueTest, IteratorInvalidationReentrancy)
{
	queue.QueueEvent<WindowClosedEvent>();

	queue.ExecuteQueuedEvents([this](Event& e) {
		processedEvents++;

		// Simulate a listener queuing a new event while in the middle of the execute loop
		if (e.GetEventType() == EventType::WindowClosed) {
			queue.QueueEvent<MouseMovedEvent>(0, 0);
		}
		});

	EXPECT_EQ(processedEvents, 1);

	queue.ExecuteQueuedEvents([this](Event& e) {
		processedEvents++;
		});

	EXPECT_EQ(processedEvents, 2);
}

TEST_F(AdvancedQueueTest, HighVolumeStressTest)
{
	const int EVENT_COUNT = 10000;
	int executionCount = 0;

	for (int i = 0; i < EVENT_COUNT; i++)
	{
		queue.QueueEvent<MouseMovedEvent>(i, i);
	}

	WindowClosedEvent syncEvent;
	EventDispatcher syncDispatcher(syncEvent);

	for (int i = 0; i < 500; i++)
	{
		syncDispatcher.DispatchEvent<WindowClosedEvent>([](WindowClosedEvent& e) { return true; });
		syncEvent.Handled = false; // We have to reset the flag so the next loop iteration can process it again
	}

	queue.ExecuteQueuedEvents([&executionCount](Event& e) {
		executionCount++;
		});

	EXPECT_EQ(executionCount, EVENT_COUNT);
}

using QueueDeathTest = AdvancedQueueTest;

TEST_F(QueueDeathTest, AllocatorOutOfMemoryRejection)
{
	// Initialize a queue with a tiny 8-byte capacity for its allocators
	EventQueue tinyQueue(8);

	EXPECT_DEATH({
		tinyQueue.QueueEvent<MouseMovedEvent>(150, 250);
		}, "Allocation exceeds buffer size! Increase capacity.");
}