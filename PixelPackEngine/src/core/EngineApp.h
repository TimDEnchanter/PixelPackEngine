#pragma once

#include "core\WindowInterface.h"

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
	};

	// Defined by client application
	EngineApp* CreateEngine();

}
