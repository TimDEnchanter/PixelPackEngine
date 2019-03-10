#include "TextureObject.h"

void pxpk::TextureObject::bindResource()
{
}

void pxpk::TextureObject::freeResource()
{
}

void pxpk::TextureObject::deleteResource()
{
}

pxpk::TextureObject::TextureObject(std::string filepath) : ObjectResource(filepath)
{
	////set default color
	//baseColor = glm::vec3(1.0, 0.0, 0.5);
}

pxpk::TextureObject::~TextureObject()
{
}