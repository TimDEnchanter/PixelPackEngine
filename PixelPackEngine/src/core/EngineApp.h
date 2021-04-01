#pragma once

#include "ApiCore.h"

namespace PixelPack {

	class PXPK_API EngineApp
	{
	public:
		EngineApp();
		virtual ~EngineApp();

		void Run();
	};

	// Defined by client application
	EngineApp* CreateEngine();

}
