#ifndef DRAW_QUEUE_H
#define DRAW_QUEUE_H

#include "DoubleBufferQueue.h"

namespace pxpk
{
	extern std::condition_variable drawCV;

	class DrawQueue : public DoubleBuffferQueue
	{
		private:
			DrawQueue() {};

		public:
			static DrawQueue& getInstance()
			{
				static DrawQueue instance;
				return instance;
			};

			DrawQueue(DrawQueue const&) = delete;
			void operator=(DrawQueue const&) = delete;

			~DrawQueue() {};

			void draw(unsigned short id)
			{
				pxpk::QueueEvent writeEvent(0, id);
				this->write(writeEvent);
			};
	};
}

#endif  // !DRAW_QUEUE_H
