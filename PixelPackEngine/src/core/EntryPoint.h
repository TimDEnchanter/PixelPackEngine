#include "Precompile.h"

#pragma once

#include "EngineApp.h"

#ifdef PXPK_PLATFORM_WINDOWS

extern PixelPack::EngineApp* PixelPack::CreateEngine();

int main(int argc, char**argv)
{
	PixelPack::Logger::Init();
	PXPK_LOG_ENGINE_INFO("Logger Initialized");

	auto engine = PixelPack::CreateEngine();
	engine->Run();
	delete engine;
}

#else
	#error Please use Windows! Other platforms not yet supported!
#endif // PXPK_PLATFORM_WINDOWS
