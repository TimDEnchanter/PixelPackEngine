#pragma once

#include <string>
#include <sstream>

#include "core\ApiCore.h"

namespace PixelPack
{
	struct PXPK_API WindowResizeEvent
	{
		int Width, Height;

		std::string ToString() const
		{
			std::stringstream stream;

			stream << "WindowResizeEvent: " << Width << ", " << Height;

			return stream.str();
		}

		template<typename OStream>
		friend OStream& operator<<(OStream& os, const WindowResizeEvent& event)
		{
			return os << event.ToString();
		}
	};
}