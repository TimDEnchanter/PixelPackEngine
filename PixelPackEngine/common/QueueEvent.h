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
			std::uint8_t type;
			std::uint16_t ID;
			std::vector<std::uint8_t> payload;

		public:
			QueueEvent(std::uint8_t inType, std::uint16_t inID) : type(inType), ID(inID) {};

			template <typename T> void writePayload(std::vector<T> &data)
			{
				std::vector<std::uint8_t> bytes = *reinterpret_cast<std::vector<std::uint8_t>*>(&data);
				payload.insert(payload.begin(), bytes.begin(), bytes.end());
			};

			template <typename T> void readPayload(T& dest)
			{
				std::copy(payload.begin(), payload.end(), &dest);
			};
	};
}

#endif  //  !QUEUE_EVENT_H