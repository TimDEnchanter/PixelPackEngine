#include "DoubleBufferQueue.h"

pxpk::QueueEvent * pxpk::DoubleBuffferQueue::write(pxpk::QueueEvent input)
{
	std::lock_guard<std::mutex> lock(queue_mutex);
	writeQueue.emplace(input);
	return & writeQueue.back();
}

pxpk::DoubleBuffferQueue::DoubleBuffferQueue()
{
}

pxpk::DoubleBuffferQueue::~DoubleBuffferQueue()
{
}

bool pxpk::DoubleBuffferQueue::isReadEmpty()
{
	return readQueue.empty();
}

pxpk::QueueEvent pxpk::DoubleBuffferQueue::read()
{
	//pxpk::QueueEvent out = readQueue.front();
	//readQueue.pop();
	//return out;

	return readQueue.front();
}

void pxpk::DoubleBuffferQueue::pop()
{
	readQueue.pop();
}

void pxpk::DoubleBuffferQueue::swap()
{
	//clear read queue
	std::queue<pxpk::QueueEvent>().swap(readQueue);
	//swap queues
	std::swap(writeQueue, readQueue);
}
