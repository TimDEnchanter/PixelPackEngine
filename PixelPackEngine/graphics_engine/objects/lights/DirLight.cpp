#include "DirLight.h"

glm::vec3 pxpk::DirLight::getDirection()
{
	return direction;
}

void pxpk::DirLight::setDirection(glm::vec3 input)
{
	direction = input;
}

void pxpk::DirLight::draw(int index)
{
	shaderPtr->use();

	std::string prefix;
	prefix = "dirLights[" + std::to_string(index) + "].";

	shaderPtr->setVec3(prefix + "direction", direction);

	shaderPtr->setVec3(prefix + "ambient", ambient);
	shaderPtr->setVec3(prefix + "diffuse", diffuse);
	shaderPtr->setVec3(prefix + "specular", specular);
}