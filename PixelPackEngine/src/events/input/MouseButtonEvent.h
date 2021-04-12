#pragma once

#include <string>
#include <sstream>

#include "core\ApiCore.h"
#include "KeyAction.h"

namespace PixelPack
{
	struct PXPK_API MouseButtonEvent
	{
		int button, modifier;
		KeyAction action;

		std::string ToString() const
		{
			std::stringstream stream;

			switch (action)
			{
			case KeyAction::PRESS:
				stream << "MouseButtonEvent: Press " << button << " (" << modifier << ")";
				break;
			case KeyAction::RELEASE:
				stream << "MouseButtonEvent: Release " << button << " (" << modifier << ")";
				break;
			}

			return stream.str();
		}

		template<typename OStream>
		friend OStream& operator<<(OStream& os, const MouseButtonEvent& event)
		{
			return os << event.ToString();
		}
	};
}