#pragma once

#ifdef PXPK_PLATFORM_WINDOWS
	#ifdef PXPK_BUILD_DLL
		#define PXPK_API __declspec(dllexport)
	#else
		#define PXPK_API __declspec(dllimport)
	#endif // PXPK_BUILD_DLL
#else
	#error Please use Windows! Other OS not yet supported!
#endif // PXPK_PLATFORM_WINDOWS
