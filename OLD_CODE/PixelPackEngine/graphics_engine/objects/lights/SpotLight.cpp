#include "SpotLight.h"

glm::vec3 pxpk::SpotLight::getDirection()
{
	return direction;
}

float pxpk::SpotLight::getInnerCutoff()
{
	return innerCutoff;
}

float pxpk::SpotLight::getOuterCutoff()
{
	return outerCutoff;
}

float pxpk::SpotLight::getConstant()
{
	return constant;
}

float pxpk::SpotLight::getLinear()
{
	return linear;
}

float pxpk::SpotLight::getQuadratic()
{
	return quadratic;
}

void pxpk::SpotLight::setDirection(glm::vec3 input)
{
	direction = input;
}

void pxpk::SpotLight::setInnerCutoff(float input)
{
	innerCutoff = input;
}

void pxpk::SpotLight::setOuterCutoff(float input)
{
	outerCutoff = input;
}

void pxpk::SpotLight::setConstant(float input)
{
	constant = input;
}

void pxpk::SpotLight::setLinear(float input)
{
	linear = input;
}

void pxpk::SpotLight::setQuadratic(float input)
{
	quadratic = input;
}

void pxpk::SpotLight::draw(int index)
{
	shaderPtr->use();

	std::string prefix;
	prefix = "spotLights[" + std::to_string(index) + "].";

	shaderPtr->setVec3(prefix + "position", position);
	shaderPtr->setVec3(prefix + "direction", direction);

	shaderPtr->setVec3(prefix + "ambient", ambient);
	shaderPtr->setVec3(prefix + "diffuse", diffuse);
	shaderPtr->setVec3(prefix + "specular", specular);

	shaderPtr->setFloat(prefix + "innerCutoff", innerCutoff);
	shaderPtr->setFloat(prefix + "outerCutoff", outerCutoff);

	shaderPtr->setFloat(prefix + "constant", constant);
	shaderPtr->setFloat(prefix + "linear", linear);
	shaderPtr->setFloat(prefix + "quadratic", quadratic);
}
