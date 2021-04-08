#pragma once

#include "core\WindowInterface.h"
#include "events\WindowCloseEvent.h"


namespace PixelPack {

	class PXPK_API EngineApp
	{
	public:
		EngineApp();
		virtual ~EngineApp();

		void Run();

	private:
		static EngineApp* ptr_Instance;
		std::unique_ptr<WindowInterface> uptr_Window;
		std::shared_ptr<entt::dispatcher> sptr_dispatcher;

		bool windowRunning = true;

	private:
		void InitDispatcher();
		void OnWindowClose(const WindowCloseEvent &);
	};

	// Defined by client application
	EngineApp* CreateEngine();

}
