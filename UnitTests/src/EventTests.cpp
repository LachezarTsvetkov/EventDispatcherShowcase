#include <gtest/gtest.h>
#include "EventDispatcher/Events/WindowEvent.h"
#include "EventDispatcher/Events/MouseEvent.h"

TEST(EventTests, StaticVsRuntimeTypeMatching)
{
	WindowResizedEvent resizeEvent(1920, 1080);
	Event& genericEvent = resizeEvent;

	// Ensure that the polymorphism has successfully mapped the base reference to the derived type
	EXPECT_EQ(genericEvent.GetEventType(), WindowResizedEvent::GetStaticType());
}

TEST(EventTests, DataIntegrity)
{
	MouseMovedEvent mouseEvent(150, 250);

	// Ensure getters return initialized values
	EXPECT_EQ(mouseEvent.GetPosX(), 150);
	EXPECT_EQ(mouseEvent.GetPosY(), 250);
}