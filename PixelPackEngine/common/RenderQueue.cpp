#include "RenderQueue.h"

pxpk::RenderQueue::RenderQueue()
{
}

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

void pxpk::RenderQueue::objLoadIndx(unsigned short id, std::vector<GLuint> input)
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
	std::vector<float> payload = { input.x, input.y, input.z };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objSetPos(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_SET_POS, id);
	std::vector<float> payload = { input.x, input.y, input.z };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objSetOrient(unsigned short id, glm::quat input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_SET_ORIENT, id);
	std::vector<float> payload = { input.w, input.x, input.y, input.z };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objSetOrientEuler(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_SET_ORIENT_EULER, id);
	std::vector<float> payload = { input.x, input.y, input.z };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objSetScale(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_SET_SCALE, id);
	std::vector<GLfloat> payload = { input };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objTrans(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_TRANSLATE, id);
	std::vector<float> payload = { input.x, input.y, input.z };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objRot(unsigned short id, glm::quat input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_ROTATE, id);
	std::vector<float> payload = { input.w, input.x, input.y, input.z };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objRotEuler(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_ROTATE_EULER, id);
	std::vector<float> payload = { input.x, input.y, input.z };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::objLookat(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_OBJ_LOOKAT, id);
	std::vector<float> payload = { input.x, input.y, input.z };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::camAdd(unsigned short id)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_CAM_ADD, id);
	this->write(writeEvent);
}

void pxpk::RenderQueue::camRemove(unsigned short id)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_CAM_REMOVE, id);
	this->write(writeEvent);
}

void pxpk::RenderQueue::camClear()
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_CAM_CLEAR, 0);
	this->write(writeEvent);
}

void pxpk::RenderQueue::camSetPos(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_CAM_SET_POS, id);
	std::vector<float> payload = { input.x, input.y, input.z };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::camSetOrient(unsigned short id, glm::quat input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_CAM_SET_ORIENT, id);
	std::vector<float> payload = { input.w, input.x, input.y, input.z };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::camSetOrientEuler(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_CAM_SET_ORIENT_EULER, id);
	std::vector<float> payload = { input.x, input.y, input.z };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::camTrans(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_CAM_TRANSLATE, id);
	std::vector<float> payload = { input.x, input.y, input.z };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::camRot(unsigned short id, glm::quat input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_CAM_ROTATE, id);
	std::vector<float> payload = { input.w, input.x, input.y, input.z };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::camRotEuler(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_CAM_ROTATE_EULER, id);
	std::vector<float> payload = { input.x, input.y, input.z };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::camLookat(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_CAM_LOOKAT, id);
	std::vector<float> payload = { input.x, input.y, input.z };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::camSetFov(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_CAM_SET_FOV, id);
	std::vector<GLfloat> payload = { input };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::camSetNear(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_CAM_SET_NEAR, id);
	std::vector<GLfloat> payload = { input };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::camSetFar(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_CAM_SET_FAR, id);
	std::vector<GLfloat> payload = { input };
	writeEvent.writePayload(payload);
	this->write(writeEvent);
}

void pxpk::RenderQueue::camSetActive(unsigned short id)
{
	pxpk::QueueEvent writeEvent(RenderType::RENDER_CAM_SET_ACTIVE, id);
	this->write(writeEvent);
}
