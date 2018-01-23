#ifndef QUEUE_EVENT_H
#define QUEUE_EVENT_H

#include <cstdint>
#include <vector>

namespace pxpk
{
	struct QueueEvent
	{
		std::uint8_t type;
		std::uint16_t ID;
		std::vector<std::uint8_t> payload;
	};
}

#endif  //  !QUEUE_EVENT_H