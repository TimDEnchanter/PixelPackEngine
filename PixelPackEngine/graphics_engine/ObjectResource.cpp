#include "ObjectResource.h"

void pxpk::ObjectResource::bindResource()
{
	LOG("Using default class for resource bind. please use an inherited class.", pxpk::INFO_LOG);
}

void pxpk::ObjectResource::freeResource()
{
	LOG("Using default class for resource freeing. please use an inherited class.", pxpk::INFO_LOG);
}

void pxpk::ObjectResource::deleteResource()
{
	LOG("Using default class for resource deletion. please use an inherited class.", pxpk::INFO_LOG);
}

pxpk::ObjectResource::ObjectResource(std::string filepath)
{
	//TEMPLATE:
	// - read file
	// - create the resource in OpenGL
	GLId = -1;
}

pxpk::ObjectResource::~ObjectResource()
{
	deleteResource();
}

GLint pxpk::ObjectResource::getGLId()
{
	return GLId;
}
