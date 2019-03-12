#include "PointLight.h"

float pxpk::PointLight::getConstant()
{
	return constant;
}

float pxpk::PointLight::getLinear()
{
	return linear;
}

float pxpk::PointLight::getQuadratic()
{
	return quadratic;
}

void pxpk::PointLight::setConstant(float input)
{
	constant = input;
}

void pxpk::PointLight::setLinear(float input)
{
	linear = input;
}

void pxpk::PointLight::setQuadratic(float input)
{
	quadratic = input;
}

void pxpk::PointLight::draw(int index)
{
	shaderPtr->use();

	std::string prefix;
	prefix = "pointLights[" + std::to_string(index) + "].";

	shaderPtr->setVec3(prefix + "position", position);
	
	shaderPtr->setVec3(prefix + "ambient", ambient);
	shaderPtr->setVec3(prefix + "diffuse", diffuse);
	shaderPtr->setVec3(prefix + "specular", specular);

	shaderPtr->setFloat(prefix + "constant", constant);
	shaderPtr->setFloat(prefix + "linear", linear);
	shaderPtr->setFloat(prefix + "quadratic", quadratic);
}
