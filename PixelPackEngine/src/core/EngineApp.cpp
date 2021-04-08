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
	}
	
	void EngineApp::OnWindowClose(const WindowCloseEvent &)
	{
		PXPK_LOG_ENGINE_INFO("Closing window");
		windowRunning = false;
	}
}