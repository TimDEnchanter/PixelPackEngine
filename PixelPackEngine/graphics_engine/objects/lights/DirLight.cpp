#include "DirLight.h"

void pxpk::DirLight::draw(int index)
{
	shaderPtr->use();

	std::string prefix;
	prefix = "dirLights[" + std::to_string(index) + "].";

	shaderPtr->setVec3(prefix + "direction", getOrientationEuler());

	shaderPtr->setVec3(prefix + "ambient", ambient);
	shaderPtr->setVec3(prefix + "diffuse", diffuse);
	shaderPtr->setVec3(prefix + "specular", specular);
}