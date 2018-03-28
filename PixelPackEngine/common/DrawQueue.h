#ifndef DRAW_QUEUE_H
#define DRAW_QUEUE_H

#include "DoubleBufferQueue.h"

namespace pxpk
{
	extern std::mutex DrawQ_Mutex;
	extern std::condition_variable DrawQ_Write_CV;
	extern std::condition_variable DrawQ_Read_CV;
	extern bool isDrawWriterReady;
	//extern bool isDrawReaderReady;

	class DrawQueue : public DoubleBuffferQueue
	{
		private:
			DrawQueue();

		public:

			static DrawQueue& getInstance()
			{
				static DrawQueue instance;
				return instance;
			};

			DrawQueue(DrawQueue const&) = delete;
			void operator=(DrawQueue const&) = delete;

			~DrawQueue();

			void draw(unsigned short);
	};
}

#endif  // !DRAW_QUEUE_H
