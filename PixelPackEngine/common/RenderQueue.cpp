#include "RenderQueue.h"

pxpk::RenderQueue::~RenderQueue()
{

}

void pxpk::RenderQueue::objAdd(unsigned short id)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_ADD, id);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objRemove(unsigned short id)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_REMOVE, id);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objClear()
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_CLEAR, 0);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objLoadVert(unsigned short id, std::vector<GLfloat> input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_LOAD_VERT, id);
	writeEvent.writePayload(input);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objLoadIndx(unsigned short id, std::vector<GLfloat> input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_LOAD_INDEX, id);
	writeEvent.writePayload(input);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objLoadColor(unsigned short id, std::vector<GLfloat> input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_LOAD_COLOR, id);
	writeEvent.writePayload(input);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objSetColor(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_SET_COLOR, id);
	writeEvent.writePayload(input);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objSetPos(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_SET_POS, id);
	writeEvent.writePayload(input);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objSetOrient(unsigned short id, glm::quat input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_SET_ORIENT, id);
	writeEvent.writePayload(input);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objSetOrientEuler(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_SET_ORIENT_EULER, id);
	writeEvent.writePayload(input);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objSetScale(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_SET_SCALE, id);
	writeEvent.writePayload(input);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objTrans(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_TRANSLATE, id);
	writeEvent.writePayload(input);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objRot(unsigned short id, glm::quat input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_ROTATE, id);
	writeEvent.writePayload(input);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objRotEuler(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_ROTATE_EULER, id);
	writeEvent.writePayload(input);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objLookat(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_LOOKAT, id);
	writeEvent.writePayload(input);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objDraw(unsigned short id)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_DRAW, id);
	this->write(writeEvent);
}
