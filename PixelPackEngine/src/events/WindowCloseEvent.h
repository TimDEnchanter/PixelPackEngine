#pragma once

#include <string>
#include <sstream>

#include "core\ApiCore.h"

namespace PixelPack
{
	struct PXPK_API WindowCloseEvent
	{
		std::string ToString() const
		{
			return "WindowCloseEvent";
		}

		template<typename OStream>
		friend OStream& operator<<(OStream& os, const WindowCloseEvent& event)
		{
			return os << event.ToString();
		}
	};
}