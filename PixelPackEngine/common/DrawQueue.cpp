#include "DrawQueue.h"

namespace pxpk
{
	std::mutex DrawQ_Mutex;
	std::condition_variable DrawQ_Write_CV;
	std::condition_variable DrawQ_Read_CV;
}

pxpk::DrawQueue::DrawQueue()
{
}

pxpk::DrawQueue::~DrawQueue()
{
}

void pxpk::DrawQueue::draw(unsigned short id)
{
	pxpk::QueueEvent writeEvent(0, id);
	this->write(writeEvent);
};