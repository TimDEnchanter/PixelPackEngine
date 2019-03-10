#include "DrawQueue.h"

namespace pxpk
{
	std::mutex DrawQ_Mutex;
	std::condition_variable DrawQ_Write_CV;
	std::condition_variable DrawQ_Read_CV;
	bool isDrawWriterReady = false;
	//bool isDrawReaderReady = false;
}

pxpk::DrawQueue::DrawQueue()
{
}

pxpk::DrawQueue::~DrawQueue()
{
}

void pxpk::DrawQueue::draw(unsigned short id)
{
	this->write(pxpk::QueueEvent(0, id));
};