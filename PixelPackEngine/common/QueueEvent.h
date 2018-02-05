#ifndef QUEUE_EVENT_H
#define QUEUE_EVENT_H

#include <cstdint>
#include <vector>
#include <algorithm>

namespace pxpk
{
	class QueueEvent
	{
		private:
			std::vector<std::uint8_t> payload;
			std::uint8_t type;
			std::uint16_t ID;

		public:
			QueueEvent(std::uint8_t inType, std::uint16_t inID) : type(inType), ID(inID) {};

			template <typename T> void writePayload(T& data)
			{
				std::uint8_t * bytes = reinterpret_cast<std::uint8_t>(data);
				payload.insert(payload.begin(), bytes, bytes + sizeof(T));
			};
			template <typename T> void readPayload(T& dest)
			{
				std::copy(payload.begin(), payload.end(), &dest);
			};
	};
}

#endif  //  !QUEUE_EVENT_H