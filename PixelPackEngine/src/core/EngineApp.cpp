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
		uptr_Window->SetEventDispatcher(sptr_Dispatcher);
	}

	EngineApp::~EngineApp()
	{

	}

	void EngineApp::Run()
	{
		while (windowRunning)
		{
			sptr_Dispatcher->update();

			for (std::shared_ptr<LayerInterface> layer : LayerStack)
			{
				layer->Update();
			}

			uptr_Window->OnUpdate();
		}
	}

	void EngineApp::PushLayer(std::shared_ptr<LayerInterface> sptr_layer)
	{
		LayerStack.PushLayer(sptr_layer);
	}

	void EngineApp::PushOverlay(std::shared_ptr<LayerInterface> sptr_overlay)
	{
		LayerStack.PushOverlay(sptr_overlay);
	}

	void EngineApp::Poplayer(std::shared_ptr<LayerInterface> sptr_layer)
	{
		LayerStack.PopLayer(sptr_layer);
	}

	void EngineApp::PopOverlay(std::shared_ptr<LayerInterface> sptr_overlay)
	{
		LayerStack.PopOverlay(sptr_overlay);
	}

	void EngineApp::InitDispatcher()
	{
		sptr_Dispatcher = std::shared_ptr<entt::dispatcher>(new entt::dispatcher());
		sptr_Dispatcher->sink<WindowCloseEvent>().connect<&EngineApp::OnWindowClose>(this);
		sptr_Dispatcher->sink<WindowResizeEvent>().connect<&EngineApp::OnWindowResize>(this);
		sptr_Dispatcher->sink<KeyEvent>().connect<&EngineApp::OnInputEvent>(this);
		sptr_Dispatcher->sink<UnicodeInputEvent>().connect<&EngineApp::OnInputEvent>(this);
		sptr_Dispatcher->sink<MouseButtonEvent>().connect<&EngineApp::OnInputEvent>(this);
		sptr_Dispatcher->sink<MouseScrollEvent>().connect<&EngineApp::OnInputEvent>(this);
		sptr_Dispatcher->sink<MouseMoveEvent>().connect<&EngineApp::OnInputEvent>(this);
	}

	void EngineApp::OnInputEvent(const entt::any& event)
	{
		for (auto iter = LayerStack.rbegin(); iter != LayerStack.rend(); ++iter)
		{
			if ((*iter)->OnEvent(event))
			{
				return;
			}
		}

		PXPK_LOG_ENGINE_WARN("Event not handled {0}", event.type().name());
	}
	
	void EngineApp::OnWindowClose(const WindowCloseEvent &event)
	{
		PXPK_LOG_ENGINE_INFO("{0}", event);
		windowRunning = false;
	}

	// TODO: Vulkan stuff for resize?
	void EngineApp::OnWindowResize(const WindowResizeEvent &event)
	{
		PXPK_LOG_ENGINE_INFO("{0}", event);
	}
}