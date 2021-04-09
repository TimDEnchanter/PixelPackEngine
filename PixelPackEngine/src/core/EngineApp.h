#pragma once

#include "core\WindowInterface.h"
#include "events\WindowCloseEvent.h"
#include "events\WindowResizeEvent.h"


namespace PixelPack {

	class PXPK_API EngineApp
	{
	public:
		EngineApp();
		virtual ~EngineApp();

		void Run();

	private:
		static EngineApp* ptr_Instance;

		#pragma warning( push )
		#pragma warning( disable : 4251 ) // Disable C4251 for these lines. It's fine since these are private
		std::unique_ptr<WindowInterface> uptr_Window;
		std::shared_ptr<entt::dispatcher> sptr_dispatcher;
		#pragma warning( pop )

		bool windowRunning = true;

	private:
		void InitDispatcher();
		void OnWindowClose(const WindowCloseEvent &event);
		void OnWindowResize(const WindowResizeEvent &event);
		void OnInputEvent(const entt::any& event);
	};

	// Defined by client application
	EngineApp* CreateEngine();

}
