#include "Camera.h"

GLfloat Camera::getFov()
{
	return fov;
}

GLfloat Camera::getNearDist()
{
	return nearDist;
}

GLfloat Camera::getFarDist()
{
	return farDist;
}

glm::vec3 Camera::getLookPoint()
{
	return lookPoint;
}

void Camera::setFov(GLfloat input)
{
	fov = input;
}

void Camera::setNearDist(GLfloat input)
{
	nearDist = input;
}

void Camera::setFarDist(GLfloat input)
{
	farDist = input;
}

void Camera::setLookPoint(glm::vec3 input)
{
	lookPoint = input;
}

void Camera::init()
{

}

void Camera::draw()
{

}

glm::mat4 Camera::getProjectionMatrix()
{
	glm::mat4 projectionOut = glm::perspective(
		glm::radians(fov),                                    // FOV angle in radians
		(float)pxpk::windowWidth / (float)pxpk::windowHeight, // aspect ratio: width/height
		nearDist,                                             // near culling distance
		farDist                                               // far culling distance
	);
	return projectionOut;
}

glm::mat4 Camera::getViewMatrix()
{
	//// create translation matrix
	//glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0), position);

	////create rotation matrix
	//glm::mat4 rotationMatrix = glm::mat4_cast(orientation);

	////ORDER IS IMPORTANT
	//glm::mat4 viewOut = rotationMatrix * translationMatrix;
	glm::mat4 viewOut = glm::lookAt(
		position, // Camera location in World Space
		lookPoint, // "look at" point
		glm::vec3(0, 1, 0)  // up vector
	);
	return viewOut;
}
