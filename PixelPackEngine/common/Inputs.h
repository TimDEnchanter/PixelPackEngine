#ifndef INPUTS_H
#define INPUTS_H

#include "../dependencies/freeGLUT/include/GL/freeglut.h"

namespace inputs_PC
{
	int mouseX;
	int mouseY;

	void initInput();

	static void mouseCallback(int, int);
}

#endif  // !INPUTS_H