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

void Camera::lookAt(glm::vec3 target, glm::vec3 up)
{
	glm::mat4 lookMat = glm::lookAt(
		position,
		target,
		up
	);

	orientation = glm::conjugate(glm::quat(lookMat));
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
	// create translation matrix (inverse of camera)
	glm::mat4 translationMatrix = glm::inverse(glm::translate(glm::mat4(1.0), position));

	//create rotation matrix (inverse of camera)
	glm::mat4 rotationMatrix = glm::inverse(glm::mat4_cast(orientation));

	//order is important
	glm::mat4 viewOut = rotationMatrix * translationMatrix;

	//glm::mat4 viewOut = glm::lookAt(
	//	position, // Camera location in World Space
	//	lookPoint, // "look at" point
	//	glm::vec3(0, 1, 0)  // up vector
	//);

	//print view matrix for debug purposes
	Logger::getInstance().log("--------------------View Matrix-------------------", LogLevel::info);
	Logger::getInstance().log("    " + std::to_string(viewOut[0][0]) + "    " + std::to_string(viewOut[0][1]) + "    " + std::to_string(viewOut[0][2]) + "    " + std::to_string(viewOut[0][3]), LogLevel::info);
	Logger::getInstance().log("    " + std::to_string(viewOut[1][0]) + "    " + std::to_string(viewOut[1][1]) + "    " + std::to_string(viewOut[1][2]) + "    " + std::to_string(viewOut[1][3]), LogLevel::info);
	Logger::getInstance().log("    " + std::to_string(viewOut[2][0]) + "    " + std::to_string(viewOut[2][1]) + "    " + std::to_string(viewOut[2][2]) + "    " + std::to_string(viewOut[2][3]), LogLevel::info);
	Logger::getInstance().log("    " + std::to_string(viewOut[3][0]) + "    " + std::to_string(viewOut[3][1]) + "    " + std::to_string(viewOut[3][2]) + "    " + std::to_string(viewOut[3][3]), LogLevel::info);
	Logger::getInstance().log("--------------------View Matrix-------------------", LogLevel::info);


	return viewOut;
}
