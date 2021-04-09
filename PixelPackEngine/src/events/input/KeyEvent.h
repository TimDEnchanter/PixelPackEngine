#pragma once

#include "KeyAction.h"

namespace PixelPack
{
	struct KeyEvent
	{
		int key, modifier;
		KeyAction action;
	};
}
