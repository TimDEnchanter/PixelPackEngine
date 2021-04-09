#include "Precompile.h"
#include "EngineApp.h"

#include "events\input\KeyEvent.h"
#include "events\input\UnicodeInputEvent.h"
#include "events\input\MouseButtonEvent.h"
#include "events\input\MouseScrollEvent.h"
#include "events\input\MouseMoveEvent.h"

namespace PixelPack
{
	EngineApp* EngineApp::ptr_Instance = nullptr;

	EngineApp::EngineApp()
	{
		PXPK_ASSERT_ENGINE(!ptr_Instance, "Engine already running!");
		ptr_Instance = this;

		InitDispatcher();

		uptr_Window = std::unique_ptr<WindowInterface>(WindowInterface::Create());
		uptr_Window->SetEventDispatcher(sptr_dispatcher);
	}

	EngineApp::~EngineApp()
	{

	}

	void EngineApp::Run()
	{
		while (windowRunning)
		{
			sptr_dispatcher->update();

			uptr_Window->OnUpdate();
		}
	}

	void EngineApp::InitDispatcher()
	{
		sptr_dispatcher = std::shared_ptr<entt::dispatcher>(new entt::dispatcher());
		sptr_dispatcher->sink<WindowCloseEvent>().connect<&EngineApp::OnWindowClose>(this);
		sptr_dispatcher->sink<WindowResizeEvent>().connect<&EngineApp::OnWindowResize>(this);
		sptr_dispatcher->sink<KeyEvent>().connect<&EngineApp::OnInputEvent>(this);
		sptr_dispatcher->sink<UnicodeInputEvent>().connect<&EngineApp::OnInputEvent>(this);
		sptr_dispatcher->sink<MouseButtonEvent>().connect<&EngineApp::OnInputEvent>(this);
		sptr_dispatcher->sink<MouseScrollEvent>().connect<&EngineApp::OnInputEvent>(this);
		sptr_dispatcher->sink<MouseMoveEvent>().connect<&EngineApp::OnInputEvent>(this);
	}
	
	void EngineApp::OnWindowClose(const WindowCloseEvent &event)
	{
		PXPK_LOG_ENGINE_INFO("Closing window");
		windowRunning = false;
	}

	// TODO: Vulkan stuff for resize?
	void EngineApp::OnWindowResize(const WindowResizeEvent &event)
	{
		PXPK_LOG_ENGINE_INFO("Resizing window {0}x{1}", event.Width, event.Height);
	}

	void EngineApp::OnInputEvent(const entt::any& event)
	{
		if (event.type() == entt::type_id<KeyEvent>())
		{
			KeyEvent& keyEvent = *(KeyEvent*) event.data();
			PXPK_LOG_ENGINE_INFO("Found Key Event ({0}) {1}", keyEvent.key, keyEvent.action);
		}
		else if (event.type() == entt::type_id<UnicodeInputEvent>())
		{
			UnicodeInputEvent& unicodeEvent = *(UnicodeInputEvent*)event.data();
			PXPK_LOG_ENGINE_INFO("Found Unicode Event ({0})", unicodeEvent.codepoint);
		}
		else if (event.type() == entt::type_id<MouseButtonEvent>())
		{
			MouseButtonEvent& mouseButtonEvent = *(MouseButtonEvent*)event.data();
			PXPK_LOG_ENGINE_INFO("Found Mouse Button Event ({0}) {1}", mouseButtonEvent.button, mouseButtonEvent.action);
		}
		else if (event.type() == entt::type_id<MouseScrollEvent>())
		{
			MouseScrollEvent& mouseScrollEvent = *(MouseScrollEvent*)event.data();
			PXPK_LOG_ENGINE_INFO("Found Mouse Scroll Event {0} {1}", mouseScrollEvent.xOffset, mouseScrollEvent.yOffset);
		}
		else if (event.type() == entt::type_id<MouseMoveEvent>())
		{
			MouseMoveEvent& mouseMoveEvent = *(MouseMoveEvent*)event.data();
			PXPK_LOG_ENGINE_INFO("Found Mouse Move Event {0} {1}", mouseMoveEvent.xPos, mouseMoveEvent.yPos);
		}
		else
		{
			PXPK_LOG_ENGINE_ERROR("Received unhandled event!");
		}
	}
}