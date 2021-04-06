#pragma once

// STDlib resources
#include <string>
#include <vector>
#include <functional>

// External Third-Party resources
#include <spdlog\spdlog.h>
#include <spdlog\sinks\stdout_color_sinks.h>

#include <entt\entt.hpp>

// Local project resources - DO NOT INCLUDE IN-DEV CODE HERE
#include "core\ApiCore.h"
#include "core\Logger.h"

#ifdef PXPK_PLATFORM_WINDOWS
#include <Windows.h>
#endif // PXPK_PLATFORM_WINDOWS
