#include "ObjectResource.h"

void pxpk::ObjectResource::createResource()
{
	LOG("Using default class for resource creation. please use an inherited class.", pxpk::INFO_LOG);
}

void pxpk::ObjectResource::deleteResource()
{
	LOG("Using default class for resource deletion. please use an inherited class.", pxpk::INFO_LOG);
}

pxpk::ObjectResource::ObjectResource(std::string filename)
{
	this->sourceFilename = filename;
	createResource();
}

pxpk::ObjectResource::~ObjectResource()
{
	deleteResource();
}

GLint pxpk::ObjectResource::getGLId()
{
	return GLId;
}
