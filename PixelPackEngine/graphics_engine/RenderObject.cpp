#include "RenderObject.h"



void RenderObject::initBuffers()
{
	//setup vertex buffer
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertexVector.size() * sizeof(TriFloat),
		&vertexVector.front(),
		GL_STATIC_DRAW
	);

	//setup element buffer
	glGenBuffers(1, &elementBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indexVector.size() * sizeof(GLuint),
		&indexVector.front(),
		GL_STATIC_DRAW
	);
}

RenderObject::RenderObject()
{
}


RenderObject::~RenderObject()
{
}

bool RenderObject::getIsHidden()
{
	return isHidden;
}

GLenum RenderObject::getDrawMode()
{
	return drawMode;
}

Color RenderObject::getObjColor()
{
	return objColor;
}

std::vector<TriFloat> RenderObject::getVertexVector()
{
	return vertexVector;
}

std::vector<GLuint> RenderObject::getIndexVector()
{
	return indexVector;
}

void RenderObject::setIsHidden(bool input)
{
	isHidden = input;
}

void RenderObject::setDrawMode(GLenum input)
{
	drawMode = input;
}

void RenderObject::setObjColor(Color input)
{
	objColor = input;
}

void RenderObject::setVertexVector(std::vector<TriFloat> input)
{
	vertexVector = input;
}

void RenderObject::setIndexVector(std::vector<GLuint> input)
{
	indexVector = input;
}


void RenderObject::init()
{
	initBuffers();
}

void RenderObject::draw()
{
	//determine sides from drawMode
	GLint sides = 3; //default to triangles
	if (drawMode == GL_QUADS) sides = 4;

	//enable use of vertex attribute
	glEnableVertexAttribArray(RenderVars::vertexAttributeID);

	//set focus to this object's vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	//point attribute to buffer
	glVertexAttribPointer(
		RenderVars::vertexAttributeID,
		sides,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//set focus to element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);

	//draw the object
	glDrawElements(
		drawMode,
		indexVector.size(),
		GL_UNSIGNED_INT,
		(void*)0
	);

	//disable use of vertex attribute
	glDisableVertexAttribArray(RenderVars::vertexAttributeID);
}