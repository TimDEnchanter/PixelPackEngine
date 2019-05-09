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

void pxpk::RenderQueue::objSetAmbient(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_SET_AMBI, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::objSetDiffuse(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_SET_DIFF, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::objSetSpecular(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_SET_SPEC, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::objSetShininess(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_SET_SHINE, id));
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

void pxpk::RenderQueue::objSetShader(unsigned short id, std::string vert, std::string frag)
{
	std::string compressed = vert + "|" + frag;
	std::vector<char> temp(compressed.c_str(), compressed.c_str() + compressed.length() + 1);
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_OBJ_SET_SHADER, id));
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

void pxpk::RenderQueue::lightRemove(unsigned short id)
{
	this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_REMOVE, id));
}

void pxpk::RenderQueue::lightClear()
{
	this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_CLEAR, 0));
}

void pxpk::RenderQueue::lightSetPos(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_SET_POS, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightSetOrient(unsigned short id, glm::quat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_SET_ORIENT, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightSetOrientEuler(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_SET_ORIENT_EULER, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightTrans(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_TRANSLATE, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightRot(unsigned short id, glm::quat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_ROTATE, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightRotEuler(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_ROTATE_EULER, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightLookat(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_LOOKAT, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightSetShader(unsigned short id, std::string vert, std::string frag)
{
	std::string compressed = vert + "|" + frag;
	std::vector<char> temp(compressed.c_str(), compressed.c_str() + compressed.length() + 1);
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_SET_SHADER, id));
	writeEvent->writePayload(temp);
}

void pxpk::RenderQueue::lightSetAmbient(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_SET_AMBI, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightSetDiffuse(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_SET_DIFF, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightSetSpecular(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_SET_SPEC, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightPointAdd(unsigned short id)
{
	this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_POINT_ADD, id));
}

void pxpk::RenderQueue::lightPointSetConstant(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_POINT_SET_CONST, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightPointSetLinear(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_POINT_SET_LIN, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightPointSetQuadratic(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_POINT_SET_QUAD, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightDirAdd(unsigned short id)
{
	this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_DIR_ADD, id));
}

void pxpk::RenderQueue::lightDirSetDir(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_DIR_SET_DIR, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightSpotAdd(unsigned short id)
{
	this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_SPOT_ADD, id));
}

void pxpk::RenderQueue::lightSpotSetDir(unsigned short id, glm::vec3 input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_SPOT_SET_DIR, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightSpotSetInCutoff(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_SPOT_SET_IN_CUTOFF, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightSpotSetOutCutoff(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_SPOT_SET_OUT_CUTOFF, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightSpotSetConstant(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_SPOT_SET_CONST, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightSpotSetLinear(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_SPOT_SET_LIN, id));
	writeEvent->writePayload(input);
}

void pxpk::RenderQueue::lightSpotSetQuadratic(unsigned short id, GLfloat input)
{
	pxpk::QueueEvent * writeEvent = this->write(pxpk::QueueEvent(RenderType::RENDER_LIGHT_SPOT_SET_QUAD, id));
	writeEvent->writePayload(input);
}
