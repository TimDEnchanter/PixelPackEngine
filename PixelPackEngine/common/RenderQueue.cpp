#include "RenderQueue.h"

namespace pxpk
{
	std::mutex RenderQ_Mutex;
	std::condition_variable RenderQ_Write_CV;
	std::condition_variable RenderQ_Read_CV;
	bool isRenderWriterReady = false;
	//bool isRenderReaderReady = false;
}

pxpk::RenderQueue::RenderQueue()
{
}

pxpk::RenderQueue::~RenderQueue()
{
}

void pxpk::RenderQueue::objAdd(unsigned short id)
{
	this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_ADD, id));
}

void pxpk::RenderQueue::objRemove(unsigned short id)
{
	this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_REMOVE, id));
}

void pxpk::RenderQueue::objClear()
{
	this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_CLEAR, 0));
}

void pxpk::RenderQueue::objSetColor(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_SET_COLOR, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::objSetMesh(unsigned short id, std::string input)
{
	std::vector<char> temp(input.c_str(), input.c_str() + input.length() + 1);
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_SET_MESH, id));
	writeEvent->writePayload(temp);
}

void pxpk::RenderQueue::objSetTex(unsigned short id, std::string input)
{
	std::vector<char> temp(input.c_str(), input.c_str() + input.length() + 1);
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_SET_TEX, id));
	writeEvent->writePayload(temp);
}

void pxpk::RenderQueue::objSetPos(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_SET_POS, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::objSetOrient(unsigned short id, glm::quat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_SET_ORIENT, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::objSetOrientEuler(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_SET_ORIENT_EULER, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::objSetScale(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_SET_SCALE, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::objTrans(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_TRANSLATE, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::objRot(unsigned short id, glm::quat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_ROTATE, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::objRotEuler(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_ROTATE_EULER, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::objLookat(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_LOOKAT, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::camAdd(unsigned short id)
{
	this->write(pxpk::QueueEvent(RenderType::RENDER_CAM_ADD, id));
}

void pxpk::RenderQueue::camRemove(unsigned short id)
{
	this->write(pxpk::QueueEvent(RenderType::RENDER_CAM_REMOVE, id));
}

void pxpk::RenderQueue::camClear()
{
	this->write(pxpk::QueueEvent(RenderType::RENDER_CAM_CLEAR, 0));
}

void pxpk::RenderQueue::camSetPos(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_CAM_SET_POS, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::camSetOrient(unsigned short id, glm::quat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_CAM_SET_ORIENT, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::camSetOrientEuler(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_CAM_SET_ORIENT_EULER, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::camTrans(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_CAM_TRANSLATE, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::camRot(unsigned short id, glm::quat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_CAM_ROTATE, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::camRotEuler(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_CAM_ROTATE_EULER, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::camLookat(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_CAM_LOOKAT, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::camSetFov(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_CAM_SET_FOV, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::camSetNear(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_CAM_SET_NEAR, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::camSetFar(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_CAM_SET_FAR, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::camSetActive(unsigned short id)
{
	this->write(pxpk::QueueEvent(RenderType::RENDER_CAM_SET_ACTIVE, id));
}
