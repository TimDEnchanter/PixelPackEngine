#pragma once

#include <string>
#include <sstream>

#include "core\ApiCore.h"

namespace PixelPack
{
	struct PXPK_API MouseMoveEvent
	{
		double xPos, yPos;

		std::string ToString() const
		{
			std::stringstream stream;

			stream << "MouseMoveEvent: " << xPos << ", " << yPos;

			return stream.str();
		}

		template<typename OStream>
		friend OStream& operator<<(OStream& os, const MouseMoveEvent& event)
		{
			return os << event.ToString();
		}
	};
}