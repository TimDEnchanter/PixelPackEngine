#ifndef DOUBLE_BUFFER_QUEUE_H
#define DOUBLE_BUFFER_QUEUE_H

#include <queue>
#include <mutex>

#include "QueueEvent.h"

namespace pxpk
{
	class DoubleBuffferQueue
	{
		private:
			std::queue<pxpk::QueueEvent> writeQueue;
			std::queue<pxpk::QueueEvent> readQueue;

			std::mutex queue_mutex;

		public:
			void write(pxpk::QueueEvent);
			pxpk::QueueEvent read();
			void swap();
	};
}

#endif  //  !DOUBLE_BUFFER_QUEUE_H