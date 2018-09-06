#include "ObjectResource.h"

template <typename T> void pxpk::ObjectResource::createResource( std::vector<T>)
{
	LOG("Using default class for resource creation. please use an inherited class.", pxpk::INFO_LOG);
}

template void pxpk::ObjectResource::createResource<GLfloat>(std::vector<GLfloat>);
template void pxpk::ObjectResource::createResource<GLuint>(std::vector<GLuint>);

void pxpk::ObjectResource::bindResource()
{
	LOG("Using default class for resource bind. please use an inherited class.", pxpk::INFO_LOG);
}

void pxpk::ObjectResource::deleteResource()
{
	LOG("Using default class for resource deletion. please use an inherited class.", pxpk::INFO_LOG);
}

pxpk::ObjectResource::ObjectResource(std::string filename)
{
	this->sourceFilename = filename;
	//TODO: read file
	//createResource(data);
}

pxpk::ObjectResource::~ObjectResource()
{
	deleteResource();
}

GLint pxpk::ObjectResource::getGLId()
{
	return GLId;
}
