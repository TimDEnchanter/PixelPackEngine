#pragma once

#include "core\WindowInterface.h"
#include "events\WindowCloseEvent.h"
#include "events\WindowResizeEvent.h"
#include "LayerStack.h"


namespace PixelPack {

	class PXPK_API EngineApp
	{
	public:
		EngineApp();
		virtual ~EngineApp();

		void Run();

		void PushLayer(std::shared_ptr<LayerInterface> sptr_layer);
		void PushOverlay(std::shared_ptr<LayerInterface> sptr_overlay);
		void Poplayer(std::shared_ptr<LayerInterface> sptr_layer);
		void PopOverlay(std::shared_ptr<LayerInterface> sptr_overlay);

	private:
		static EngineApp* ptr_Instance;

		#pragma warning( push )
		#pragma warning( disable : 4251 ) // Disable C4251 for these lines. It's fine since these are private
		std::unique_ptr<WindowInterface> uptr_Window;
		std::shared_ptr<entt::dispatcher> sptr_Dispatcher;
		LayerStack LayerStack;
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
