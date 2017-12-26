#include "RenderObject.h"



void RenderObject::initBuffers()
{
	
	Logger::getInstance().log("loading buffers", LogLevel::info);
	/*
	//setup vertex buffer
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertexVector.size() * sizeof(glm::vec3),
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
	*/

	//test cube
	static const GLfloat buffer_data[] =
	{
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f,-1.0f, // triangle 2 : begin
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triangle 2 : end
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	std::vector<GLfloat> vecTest(buffer_data, buffer_data + sizeof buffer_data / sizeof buffer_data[0]);

	// This will identify our vertex buffer
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	//glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, 
		vecTest.size() * sizeof(GLfloat),
		&vecTest[0], 
		GL_STATIC_DRAW);

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

std::vector<glm::vec3> RenderObject::getVertexVector()
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

void RenderObject::setVertexVector(std::vector<glm::vec3> input)
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
	Logger::getInstance().log("drawing Object", LogLevel::info);


	/*
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

	
	//set focus to element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);

	//draw the object
	glDrawElements(
		drawMode,
		indexVector.size(),
		GL_UNSIGNED_SHORT,
		(void*)0
	);
	

	//disable use of vertex attribute
	glDisableVertexAttribArray(RenderVars::vertexAttributeID);
	*/

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares
	glDisableVertexAttribArray(0);
}