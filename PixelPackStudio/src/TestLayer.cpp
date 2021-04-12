#include "TestLayer.h"

#include <spdlog\spdlog.h>

TestLayer::TestLayer(bool enabled)
{
	Enabled = enabled;
	sptr_Dispatcher = std::shared_ptr<entt::dispatcher>(new entt::dispatcher());
}

TestLayer::~TestLayer()
{
}

void TestLayer::Attatch()
{
	PXPK_LOG_INFO("TestLayer::Attatch");
	sptr_Dispatcher->sink<PixelPack::KeyEvent>().connect<&TestLayer::OnKeyEvent>(this);
}

void TestLayer::Detach()
{
	sptr_Dispatcher->sink<PixelPack::KeyEvent>().disconnect<&TestLayer::OnKeyEvent>(this);
}

void TestLayer::Update()
{
	//PXPK_LOG_INFO("TestLayer::Update");
	sptr_Dispatcher->update();
}

void TestLayer::SetEnabled(bool enabled)
{
	Enabled = enabled;
}

bool TestLayer::IsEnabled() const
{
	return Enabled;
}

bool TestLayer::OnEvent(entt::any event)
{
	if (event.type() == entt::type_id<PixelPack::KeyEvent>() && !sptr_Dispatcher->sink<PixelPack::KeyEvent>().empty())
	{
		PixelPack::KeyEvent& keyEvent = *(PixelPack::KeyEvent*)event.data();
		sptr_Dispatcher->enqueue<PixelPack::KeyEvent>(keyEvent);
		return true;
	}
	return false;
}

void TestLayer::OnKeyEvent(const PixelPack::KeyEvent& event)
{
	PXPK_LOG_INFO("TestLayer::OnKeyEvent {0}", event);
}