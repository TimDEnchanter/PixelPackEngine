#include "MeshObject.h"

void pxpk::MeshObject::createResource(
	std::vector<GLfloat> vertexData, 
	std::vector<GLfloat> colorData, 
	std::vector<GLfloat> normalData, 
	std::vector<GLfloat> uvData, 
	std::vector<GLuint>  indexData)
{
	LOG("Creating model VBOs in OpenGL", pxpk::INFO_LOG);

	//create vertex buffer
	glGenBuffers(1, &vertexID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexID);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertexData.size() * sizeof(GLfloat),
		&vertexData.front(),
		GL_STATIC_DRAW
	);

	//create color buffer
	glGenBuffers(1, &colorID);
	glBindBuffer(GL_ARRAY_BUFFER, colorID);
	glBufferData(
		GL_ARRAY_BUFFER,
		colorData.size() * sizeof(GLfloat),
		&colorData.front(),
		GL_STATIC_DRAW
	);

	//create normal buffer
	glGenBuffers(1, &normalID);
	glBindBuffer(GL_ARRAY_BUFFER, normalID);
	glBufferData(
		GL_ARRAY_BUFFER,
		normalData.size() * sizeof(GLfloat),
		&normalData.front(),
		GL_STATIC_DRAW
	);

	//create UV buffer
	glGenBuffers(1, &uvID);
	glBindBuffer(GL_ARRAY_BUFFER, uvID);
	glBufferData(
		GL_ARRAY_BUFFER,
		uvData.size() * sizeof(GLfloat),
		&uvData.front(),
		GL_STATIC_DRAW
	);

	//create index buffer
	glGenBuffers(1, &indexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indexData.size() * sizeof(GLuint),
		&indexData.front(),
		GL_STATIC_DRAW
	);
}

void pxpk::MeshObject::bindResource()
{
	//enable use of vertex attribute
	glEnableVertexAttribArray(pxpk::vertexAttributeID);
	//set focus to this object's vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexID);
	//point attribute to buffer
	glVertexAttribPointer(
		pxpk::vertexAttributeID,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//enable use of color attribute
	glEnableVertexAttribArray(pxpk::colorAttributeID);
	//set focus to this object's color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorID);
	//point attribute to buffer
	glVertexAttribPointer(
		pxpk::colorAttributeID,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//enable use of normal attribute
	glEnableVertexAttribArray(pxpk::normalAttributeID);
	//set focus to this object's normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, normalID);
	//point attribute to buffer
	glVertexAttribPointer(
		pxpk::normalAttributeID,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//enable use of UV attribute
	glEnableVertexAttribArray(pxpk::UVAttributeID);
	//set focus to this object's UV buffer
	glBindBuffer(GL_ARRAY_BUFFER, uvID);
	//point attribute to buffer
	glVertexAttribPointer(
		pxpk::UVAttributeID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
}

void pxpk::MeshObject::freeResource()
{
	glDisableVertexAttribArray(pxpk::vertexAttributeID);
	glDisableVertexAttribArray(pxpk::colorAttributeID);
	glDisableVertexAttribArray(pxpk::normalAttributeID);
	glDisableVertexAttribArray(pxpk::UVAttributeID);
}

void pxpk::MeshObject::deleteResource()
{
	GLuint buffers[5] = { vertexID, colorID, normalID, uvID, indexID };
	glDeleteBuffers(5, buffers);
}

pxpk::MeshObject::MeshObject(std::string filepath) : ObjectResource(filepath)
{
	//read file
	//pass data to OpenGL
}

pxpk::MeshObject::~MeshObject()
{
}
