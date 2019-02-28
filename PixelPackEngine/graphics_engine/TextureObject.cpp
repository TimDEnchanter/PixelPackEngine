#include "TextureObject.h"

void pxpk::TextureObject::bindResource()
{
	//GLuint colorID = glGetUniformLocation(programID, "objColor");
	//glUniform3fv(colorID, 1, glm::value_ptr(baseColor));
}

void pxpk::TextureObject::freeResource()
{
}

void pxpk::TextureObject::deleteResource()
{
}

pxpk::TextureObject::TextureObject(std::string filepath) : ObjectResource(filepath)
{
	//set default color
	baseColor = glm::vec3(1.0, 0.0, 0.5);
}

pxpk::TextureObject::~TextureObject()
{
}

glm::vec3 pxpk::TextureObject::getBaseColor()
{
	return baseColor;
}

GLuint pxpk::TextureObject::getProgramID()
{
	return programID;
}

void pxpk::TextureObject::setBaseColor(glm::vec3 input)
{
	baseColor = input;
}

void pxpk::TextureObject::setProgramID(GLuint input)
{
	programID = input;
}
