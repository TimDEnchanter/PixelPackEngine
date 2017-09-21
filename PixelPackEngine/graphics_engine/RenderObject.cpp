#include "RenderObject.h"



RenderObject::RenderObject()
{
}


RenderObject::~RenderObject()
{
}


void RenderObject::draw()
{
	//set color to draw with
	glColor3f(objColor.getRed(), objColor.getGreen(), objColor.getBlue());

	//start drawing
	glBegin(drawMode);



	//stop drawing
	glEnd();
}