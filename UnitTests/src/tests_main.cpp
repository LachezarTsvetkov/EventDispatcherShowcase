#include <gtest/gtest.h>

#include "EventDispatcher/Dispatcher.h"
#include "EventDispatcher/Events/WindowEvent.h"

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

class TestEvent : public Event {
public:
	TestEvent() {}

	EVENT_CLASS_TYPE(Custom);
};

class EventDispatcherTest : public testing::Test {
protected:
	bool wasHandlerCalled = false;

	void OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.DispatchEvent<TestEvent>(BIND_EVENT_FN(EventDispatcherTest::EventHandler));
	}

	bool EventHandler(TestEvent& e)
	{
		wasHandlerCalled = true;
		return true;
	}
};

TEST_F(EventDispatcherTest, BasicEventDispatching)
{
	TestEvent event;
	OnEvent(event);
	EXPECT_TRUE(wasHandlerCalled);
}
int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}