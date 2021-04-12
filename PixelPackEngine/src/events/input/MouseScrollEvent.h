#pragma once

#include <string>
#include <sstream>

#include "core\ApiCore.h"

namespace PixelPack
{
	struct PXPK_API MouseScrollEvent
	{
		double xOffset, yOffset;

		std::string ToString() const
		{
			std::stringstream stream;

			stream << "MouseScrollEvent: " << xOffset << ", " << yOffset;

			return stream.str();
		}

		template<typename OStream>
		friend OStream& operator<<(OStream& os, const MouseScrollEvent& event)
		{
			return os << event.ToString();
		}
	};
}