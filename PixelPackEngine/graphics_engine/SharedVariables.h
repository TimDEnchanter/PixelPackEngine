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

namespace pxpk {
	//shader attribute indices
	const GLuint vertexAttributeID = 0;
	const GLuint colorAttributeID = 1;
	const GLuint UVAttributeID = 2;

	//window size
	const int windowWidth = 1280;
	const int windowHeight = 720;
}

#endif // !SHARED_VARIABLES_H

