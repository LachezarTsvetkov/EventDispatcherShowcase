#include <gtest/gtest.h>
#include "EventDispatcher/Dispatcher.h"
#include "EventDispatcher/Events/WindowEvent.h"
#include "EventDispatcher/Events/MouseEvent.h"
#include "EventDispatcher/Events/KeyEvent.h"
#include "MemoryProfiler.h"

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
	queue.QueueEvent<MouseMovedEvent>(10, 10);
	queue.QueueEvent<KeyPressedEvent>(65);
	queue.QueueEvent<WindowClosedEvent>();

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
	queue.QueueEvent<MouseMovedEvent>(10, 10);

	WindowClosedEvent syncEvent;
	RouteQueuedEvent(syncEvent);

	queue.ExecuteQueuedEvents([this](Event& e) { RouteQueuedEvent(e); });

	ASSERT_EQ(executionOrder.size(), 2);
	EXPECT_EQ(executionOrder[0], EventType::WindowClosed);
	EXPECT_EQ(executionOrder[1], EventType::MouseMoved);
}


TEST_F(EventQueueTestFixture, ZeroHeapAllocationDuringFrame)
{
	// Allocate the 2MB buffer here before taking a snapshot of the memory state
	EventQueue memoryQueue;

	uint32_t allocationsBefore = s_Metrics.TotalAllocated;

	memoryQueue.QueueEvent<MouseMovedEvent>(10, 10);
	memoryQueue.QueueEvent<KeyPressedEvent>(65);
	memoryQueue.QueueEvent<WindowClosedEvent>();

	memoryQueue.ExecuteQueuedEvents([](Event& e) {
		// We do not perform any heap allocations in this callback, as all events are allocated in the pre-allocated buffer.
	});

	uint32_t allocationsAfter = s_Metrics.TotalAllocated;
	uint32_t heapAllocationsDuringFrame = allocationsAfter - allocationsBefore;

	// Ensure the engine bypassed the OS heap
	EXPECT_EQ(heapAllocationsDuringFrame, 0);
}


TEST_F(EventQueueTestFixture, MultithreadedConcurrencyTest)
{
	const int THREAD_COUNT = 4;
	const int EVENTS_PER_THREAD = 2500;
	const int TOTAL_EVENTS = THREAD_COUNT * EVENTS_PER_THREAD;

	EventQueue concurrentQueue(5 * 1024 * 1024, TOTAL_EVENTS);
	std::vector<std::thread> workers;

	// Spawn 4 parallel threads that instantly start firing events into the queue
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		workers.emplace_back([&concurrentQueue, EVENTS_PER_THREAD]() {
			for (int j = 0; j < EVENTS_PER_THREAD; j++)
			{
				concurrentQueue.QueueEvent<MouseMovedEvent>(j, j);
			}
			});
	}

	// Join the threads to wait for all background queuing to finish
	for (auto& worker : workers)
	{
		worker.join();
	}

	// Process the frame and count the results
	int processedCount = 0;
	concurrentQueue.ExecuteQueuedEvents([&processedCount](Event& e) {
		processedCount++;
		});


	EXPECT_EQ(processedCount, TOTAL_EVENTS);
} 