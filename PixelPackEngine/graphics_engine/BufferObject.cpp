#include "BufferObject.h"

template <typename T> void pxpk::BufferObject::createResource(std::vector<T> data)
{
	LOG("Creating buffer object in OpenGL", pxpk::INFO_LOG);

	glGenBuffers(1, &GLId);
	glBindBuffer(GL_ARRAY_BUFFER, GLId);
	glBufferData(
		GL_ARRAY_BUFFER,
		data.size() * sizeof(T),
		&data.front(),
		GL_STATIC_DRAW
	);
}

template void pxpk::BufferObject::createResource<GLfloat>(std::vector<GLfloat>);
template void pxpk::BufferObject::createResource<GLuint>(std::vector<GLuint>);

void pxpk::BufferObject::bindResource()
{
	glBindBuffer(GL_ARRAY_BUFFER, GLId);
}

void pxpk::BufferObject::deleteResource()
{
	glDeleteBuffers(1, &GLId);
}

pxpk::BufferObject::BufferObject(std::string filename) : ObjectResource(filename)
{
}

pxpk::BufferObject::~BufferObject()
{
}
