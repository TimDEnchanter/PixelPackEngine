#include "RenderObject.h"

glm::quat pxpk::RenderObject::quatToVector(glm::vec3 start, glm::vec3 dest)
{
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	float cosTheta = glm::dot(start, dest);
	glm::vec3 rotationAxis;
	if (cosTheta < -1 + 0.001f)
	{
		//rotate 180 around any perpendicular vector
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (std::powf(rotationAxis.length(), 2.0f) < 0.01f) //check if arbitrary axis is parallel
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = glm::normalize(rotationAxis);
		return glm::angleAxis(glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = glm::cross(start, dest);
	float s = sqrt((1.0f + cosTheta) * 2.0f);
	float invs = 1.0f / s;

	return glm::quat(
		s*0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}

pxpk::RenderObject::RenderObject()
{
}

pxpk::RenderObject::RenderObject(const RenderObject & input)
{
	position = input.position;
	orientation = input.orientation;
	scale = input.scale;
	drawMode = input.drawMode;
	meshPtr = input.meshPtr;
}


pxpk::RenderObject::~RenderObject()
{
}

GLenum pxpk::RenderObject::getDrawMode()
{
	return drawMode;
}

glm::vec3 pxpk::RenderObject::getPosition()
{
	return position;
}

glm::quat pxpk::RenderObject::getOrientaion()
{
	return orientation;
}

glm::vec3 pxpk::RenderObject::getOrientationEuler()
{
	return glm::eulerAngles(orientation);
}

glm::vec3 pxpk::RenderObject::getScale()
{
	return scale;
}

std::shared_ptr<pxpk::MeshObject> pxpk::RenderObject::getMeshPtr()
{
	return meshPtr;
}

std::shared_ptr<pxpk::TextureObject> pxpk::RenderObject::getTexturePtr()
{
	return texPtr;
}

GLuint pxpk::RenderObject::getProgramID()
{
	return programID;
}

glm::mat4 pxpk::RenderObject::getModelMatrix()
{
	// create translation matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0), position);

	//create scale matrix
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), scale);

	//create rotation matrix
	glm::mat4 rotationMatrix = glm::mat4_cast(orientation);

	//multiply together **ORDER IS IMPORTANT**
	glm::mat4 modelOut =  translationMatrix * rotationMatrix * scaleMatrix;
	return modelOut;
}

void pxpk::RenderObject::setDrawMode(GLenum input)
{
	drawMode = input;
}

void pxpk::RenderObject::setObjColor(glm::vec3 input)
{
	texPtr->setBaseColor(input);
	texPtr->setProgramID(programID);
}

void pxpk::RenderObject::setPosition(glm::vec3 input)
{
	position = input;
}

void pxpk::RenderObject::setOrientation(glm::quat input)
{
	orientation = input;
}

void pxpk::RenderObject::setOrientationEuler(glm::vec3 eulerInput)
{
	orientation = glm::quat(eulerInput);
}

void pxpk::RenderObject::setScale(glm::vec3 input)
{
	scale = input;
}

void pxpk::RenderObject::setMeshPtr(std::shared_ptr<MeshObject> input)
{
	meshPtr = input;
}

void pxpk::RenderObject::setTexturePtr(std::shared_ptr<pxpk::TextureObject> input)
{
	texPtr = input;
}

void pxpk::RenderObject::setProgramID(GLuint input)
{
	programID = input;
}

void pxpk::RenderObject::translate(glm::vec3 input)
{
	position += input;
}

void pxpk::RenderObject::rotateEuler(glm::vec3 input)
{
	glm::quat inQuat = glm::quat(input);
	orientation = inQuat * orientation;
}

void pxpk::RenderObject::rotate(glm::quat input)
{
	orientation = input * orientation;
}

void pxpk::RenderObject::lookAt(glm::vec3 target)
{
	glm::vec3 targetVector = target - position;
	glm::quat rotation = quatToVector(glm::vec3(0.0f,0.0f,1.0f), targetVector);

	glm::vec3 targetUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::cross(targetVector, targetUp);
	targetUp = glm::cross(right, targetVector);
	glm::vec3 newUp = rotation * glm::vec3(0.0f, 1.0f, 0.0f);

	glm::quat upRotation = quatToVector(newUp, targetUp);

	//ORDER IMPORTANT
	orientation = upRotation * rotation;
}

void pxpk::RenderObject::draw()
{	
	//bind mesh data
	meshPtr->bindResource();

	//bind texture data
	texPtr->bindResource();
	
	//set focus to element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshPtr->getIndexID());

	//draw the object
	glDrawElements(
		drawMode,
		meshPtr->getIndexSize(),
		GL_UNSIGNED_INT,
		(void*)0
	);

	//done with object, release
	meshPtr->freeResource();
}