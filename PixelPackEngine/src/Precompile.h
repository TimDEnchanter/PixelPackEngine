#pragma once

// STDlib resources
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <sstream>
#include <optional>
#include <cstdint>
#include <algorithm>

// External Third-Party resources
#include <spdlog\spdlog.h>
#include <spdlog\sinks\stdout_color_sinks.h>
#include <spdlog\fmt\ostr.h>

#include <entt\entt.hpp>

#include <vulkan\vulkan.h>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULCAN
#include <GLFW\glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

// Local project resources - DO NOT INCLUDE IN-DEV CODE HERE
#include "core\ApiCore.h"
#include "core\Logger.h"

#ifdef PXPK_PLATFORM_WINDOWS
#include <Windows.h>
#endif // PXPK_PLATFORM_WINDOWS
