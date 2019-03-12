#include "Light.h"

std::shared_ptr<pxpk::ShaderObject> pxpk::Light::getShaderPtr()
{
	return shaderPtr;
}

glm::vec3 pxpk::Light::getAmbient()
{
	return ambient;
}

glm::vec3 pxpk::Light::getDiffuse()
{
	return diffuse;
}

glm::vec3 pxpk::Light::getSpecular()
{
	return specular;
}

void pxpk::Light::setShaderPtr(std::shared_ptr<pxpk::ShaderObject> input)
{
	shaderPtr = input;
}

void pxpk::Light::setAmbient(glm::vec3 input)
{
	ambient = input;
}

void pxpk::Light::setDiffuse(glm::vec3 input)
{
	diffuse = input;
}

void pxpk::Light::setSpecular(glm::vec3 input)
{
	specular = input;
}

void pxpk::Light::draw(int index)
{
	//Empty virtual function. use a derived class
}
