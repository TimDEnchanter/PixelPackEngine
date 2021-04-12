#pragma once

#include <string>
#include <sstream>

#include "core\ApiCore.h"

namespace PixelPack
{
	struct PXPK_API UnicodeInputEvent
	{
		unsigned int codepoint;

		std::string ToString() const
		{
			std::stringstream stream;

			stream << "UnicodeInputEvent: " << codepoint;

			return stream.str();
		}

		template<typename OStream>
		friend OStream& operator<<(OStream& os, const UnicodeInputEvent& event)
		{
			return os << event.ToString();
		}
	};
}