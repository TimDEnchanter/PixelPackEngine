#include "RenderObject.h"



RenderObject::RenderObject()
{
}


RenderObject::~RenderObject()
{
}


void RenderObject::draw()
{
	//determine draw mode
	GLint sides = 3; //default to triangles
	if (drawMode == GL_TRIANGLES) sides = 3;
	else if (drawMode == GL_QUADS) sides = 4;


	//enable use of arrays
	glEnableClientState(GL_VERTEX_ARRAY);

	//setup arrays
	glVertexPointer(sides, GL_FLOAT, sides * sizeof(GLfloat), vertexVector.data());


	//start drawing
	glPushMatrix();
		
		//set color to draw with
		glColor3f(objColor.getRed(), objColor.getGreen(), objColor.getBlue());

		//draw verts
		glDrawArrays(drawMode, 0, vertexVector.size());

	//stop drawing
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
}