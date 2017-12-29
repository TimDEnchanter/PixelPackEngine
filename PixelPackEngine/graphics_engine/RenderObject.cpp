#include "RenderObject.h"



void RenderObject::initBuffers()
{
	
	Logger::getInstance().log("loading buffers", LogLevel::info);
	
	//setup vertex buffer
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertexVector.size() * sizeof(GLfloat),
		&vertexVector.front(),
		GL_STATIC_DRAW
	);

	//setup color buffer
	glGenBuffers(1, &colorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	glBufferData(
		GL_ARRAY_BUFFER,
		colorVector.size() * sizeof(GLfloat),
		&colorVector.front(),
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

glm::vec3 RenderObject::getObjColor()
{
	return objColor;
}

std::vector<GLfloat> RenderObject::getVertexVector()
{
	return vertexVector;
}

std::vector<GLuint> RenderObject::getIndexVector()
{
	return indexVector;
}

std::vector<GLfloat> RenderObject::getColorVector()
{
	return colorVector;
}

glm::mat4 RenderObject::getModelMatrix()
{
	return glm::mat4(1.0);
}

void RenderObject::setIsHidden(bool input)
{
	isHidden = input;
}

void RenderObject::setDrawMode(GLenum input)
{
	drawMode = input;
}

void RenderObject::setObjColor(glm::vec3 input)
{
	objColor = input;
}

void RenderObject::setVertexVector(std::vector<GLfloat> input)
{
	vertexVector = input;
}

void RenderObject::setIndexVector(std::vector<GLuint> input)
{
	indexVector = input;
}

void RenderObject::setColorVector(std::vector<GLfloat> input)
{
	colorVector = input;
}


void RenderObject::loadOBJ(std::string filePath)
{
}

void RenderObject::init()
{
	initBuffers();
}

void RenderObject::draw()
{
	//Logger::getInstance().log("drawing Object", LogLevel::info);
	
	//determine sides from drawMode
	GLint sides = 3; //default to triangles
	//if (drawMode == GL_QUADS) sides = 4;

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


	//enable use of color attribute
	glEnableVertexAttribArray(RenderVars::colorAttributeID);

	//set focus to this object's color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);

	//point to buffer
	glVertexAttribPointer(
		RenderVars::colorAttributeID,
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