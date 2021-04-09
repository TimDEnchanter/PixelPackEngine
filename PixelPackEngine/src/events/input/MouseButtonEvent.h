#pragma once

#include "KeyAction.h"

namespace PixelPack
{
	struct MouseButtonEvent
	{
		int button, modifier;
		KeyAction action;
	};
}