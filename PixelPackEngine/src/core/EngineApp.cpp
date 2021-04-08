#include "Precompile.h"
#include "EngineApp.h"

namespace PixelPack
{
	EngineApp* EngineApp::ptr_Instance = nullptr;

	EngineApp::EngineApp()
	{
		PXPK_ASSERT_ENGINE(!ptr_Instance, "Engine already running!");

		ptr_Instance = this;
		uptr_Window = std::unique_ptr<WindowInterface>(WindowInterface::Create());
	}

	EngineApp::~EngineApp()
	{

	}

	void EngineApp::Run()
	{
		while (true)
		{
			uptr_Window->OnUpdate();
		}
	}
}