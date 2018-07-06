#include "Inputs.h"

namespace inputs_PC
{
	int mouseX = 0;
	int mouseY = 0;
}

void inputs_PC::initInput()
{
	glutPassiveMotionFunc(inputs_PC::mouseCallback);
}

void inputs_PC::mouseCallback(int inX, int inY)
{
	inputs_PC::mouseX = inX;
	inputs_PC::mouseY = inY;
}