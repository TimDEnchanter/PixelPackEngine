#ifndef SHARED_VARIABLES_H
#define SHARED_VARIABLES_H

#include "GL/glew.h"
#include "GL/freeglut.h"

/*
Shared Variables

Set of variables that will be used across the engine.
Also contains OpenGL library includes.

****INCLUDE THIS FOR ALL CLASSES IN GRAPHICS_ENGINE****
*/

namespace RenderVars {
	//shader attribute indices
	const GLuint vertexAttributeID = 0;
	const GLuint normalAttributeID = 1;
	const GLuint colorAttributeID = 2;
}

#endif // !SHARED_VARIABLES_H

