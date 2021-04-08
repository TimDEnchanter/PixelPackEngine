#include "Precompile.h"
#include "EngineApp.h"

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
			uptr_Window->OnUpdate();
		}
	}

	void EngineApp::InitDispatcher()
	{
		sptr_dispatcher = std::shared_ptr<entt::dispatcher>(new entt::dispatcher());
		sptr_dispatcher->sink<WindowCloseEvent>().connect<&EngineApp::OnWindowClose>(this);
		sptr_dispatcher->sink<WindowResizeEvent>().connect<&EngineApp::OnWindowResize>(this);
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
}