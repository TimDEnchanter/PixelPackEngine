#include "Precompile.h"

#pragma once

#include "EngineApp.h"

#ifdef PXPK_PLATFORM_WINDOWS

extern PixelPack::EngineApp* PixelPack::CreateEngine();

int main(int argc, char**argv)
{
	PixelPack::Logger::Init();
	PXPK_LOG_ENGINE_WARN("Initialized");
	int var = 5;
	PXPK_LOG_INFO("Initialized: var={0}", var);

	auto engine = PixelPack::CreateEngine();
	engine->Run();
	delete engine;
}

#else
	#error Please use Windows! Other OS not yet supported!
#endif // PXPK_PLATFORM_WINDOWS
