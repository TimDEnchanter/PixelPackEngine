#include "ScreenSpaceElement.h"

glm::vec2 pxpk::ScreenSpaceElement::getPosition()
{
	return position;
}

glm::vec2 pxpk::ScreenSpaceElement::getPixelSize()
{
	return pixelSize;
}

std::shared_ptr<pxpk::ShaderObject> pxpk::ScreenSpaceElement::getShaderPtr()
{
	return shaderPtr;
}

void pxpk::ScreenSpaceElement::setPosition(glm::vec2 input)
{
	position = input;
}

void pxpk::ScreenSpaceElement::setPixelSize(glm::vec2 input)
{
	pixelSize = input;
}

void pxpk::ScreenSpaceElement::setShaderPtr(std::shared_ptr<pxpk::ShaderObject> input)
{
	shaderPtr = input;
}
