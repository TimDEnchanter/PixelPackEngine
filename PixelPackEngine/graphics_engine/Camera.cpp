#include "Camera.h"

GLfloat pxpk::Camera::getFov()
{
	return fov;
}

GLfloat pxpk::Camera::getNearDist()
{
	return nearDist;
}

GLfloat pxpk::Camera::getFarDist()
{
	return farDist;
}

void pxpk::Camera::setFov(GLfloat input)
{
	fov = input;
}

void pxpk::Camera::setNearDist(GLfloat input)
{
	nearDist = input;
}

void pxpk::Camera::setFarDist(GLfloat input)
{
	farDist = input;
}

void pxpk::Camera::lookAt(glm::vec3 target, glm::vec3 up)
{
	glm::mat4 lookMat = glm::lookAt(
		position,
		target,
		up
	);

	orientation = glm::conjugate(glm::quat(lookMat));
}

void pxpk::Camera::init()
{

}

void pxpk::Camera::draw()
{

}

glm::mat4 pxpk::Camera::getProjectionMatrix()
{
	glm::mat4 projectionOut = glm::perspective(
		glm::radians(fov),                                    // FOV angle in radians
		(float)pxpk::windowWidth / (float)pxpk::windowHeight, // aspect ratio: width/height
		nearDist,                                             // near culling distance
		farDist                                               // far culling distance
	);
	return projectionOut;
}

glm::mat4 pxpk::Camera::getViewMatrix()
{
	// create translation matrix (inverse of camera)
	glm::mat4 translationMatrix = glm::inverse(glm::translate(glm::mat4(1.0), position));

	//create rotation matrix (inverse of camera)
	glm::mat4 rotationMatrix = glm::inverse(glm::mat4_cast(orientation));

	//order is important
	glm::mat4 viewOut = rotationMatrix * translationMatrix;

	return viewOut;
}
