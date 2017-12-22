#ifndef SHARED_VARIABLES_H
#define SHARED_VARIABLES_H

#include "../dependencies/GLEW/include/GL/glew.h"
#include "../dependencies/freeGLUT/include/GL/freeglut.h"

/*
Shared Variables

Set of variables that will be used across the engine.
Also contains OpenGL library includes.

****INCLUDE THIS FOR ALL CLASSES IN GRAPHICS_ENGINE****
*/

namespace RenderVars {
	//shader attribute indices
	const GLuint vertexAttributeID = 0;
	const GLuint nomalAttributeID = 1;
	const GLuint colorAttributeID = 2;
}

#endif // !SHARED_VARIABLES_H

