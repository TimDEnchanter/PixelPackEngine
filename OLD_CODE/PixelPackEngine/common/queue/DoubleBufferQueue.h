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

		protected:
			pxpk::QueueEvent * write(pxpk::QueueEvent);

			DoubleBuffferQueue();
			~DoubleBuffferQueue();

		public:
			bool isReadEmpty();
			pxpk::QueueEvent read();
			void pop();
			void swap();
	};
}

#endif  //  !DOUBLE_BUFFER_QUEUE_H