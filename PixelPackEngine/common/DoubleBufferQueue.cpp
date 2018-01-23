#include "DoubleBufferQueue.h"

void pxpk::DoubleBuffferQueue::write(pxpk::QueueEvent)
{
}

pxpk::QueueEvent pxpk::DoubleBuffferQueue::read()
{
	return pxpk::QueueEvent();
}

void pxpk::DoubleBuffferQueue::swap()
{
}
