#pragma once

#include "EngineApp.h"

#ifdef PXPK_PLATFORM_WINDOWS

extern PixelPack::EngineApp* PixelPack::CreateEngine();

int main(int argc, char**argv)
{
	auto engine = PixelPack::CreateEngine();
	engine->Run();
	delete engine;
}

#else
	#error Please use Windows! Other OS not yet supported!
#endif // PXPK_PLATFORM_WINDOWS
