#include "DoubleBufferQueue.h"

void pxpk::DoubleBuffferQueue::write(pxpk::QueueEvent input)
{
	std::lock_guard<std::mutex> lock(queue_mutex);
	writeQueue.push(input);
}

pxpk::QueueEvent pxpk::DoubleBuffferQueue::read()
{
	pxpk::QueueEvent out = readQueue.front();
	readQueue.pop();
	return out;
}

void pxpk::DoubleBuffferQueue::swap()
{
	//clear read queue
	std::queue<pxpk::QueueEvent>().swap(readQueue);
	//swap queues
	std::swap(writeQueue, readQueue);
}
