#pragma once

#include <string>
#include <sstream>

#include "core\ApiCore.h"
#include "KeyAction.h"

namespace PixelPack
{
	struct PXPK_API KeyEvent
	{
		int key, modifier;
		KeyAction action;

		std::string ToString() const
		{
			std::stringstream stream;

			switch (action)
			{
			case KeyAction::PRESS:
				stream << "keyEvent: Press " << key << " (" << modifier << ")";
				break;
			case KeyAction::REPEAT:
				stream << "keyEvent: Repeat " << key << " (" << modifier << ")";
				break;
			case KeyAction::RELEASE:
				stream << "keyEvent: Release " << key << " (" << modifier << ")";
				break;
			}
			
			return stream.str();
		}

		template<typename OStream>
		friend OStream& operator<<(OStream& os, const KeyEvent& event)
		{
			return os << event.ToString();
		}
	};
}
