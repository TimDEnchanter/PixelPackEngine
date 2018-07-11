#include "Inputs.h"

pxpk::InputsPC::InputsPC()
{
	pxpk::inputsPCInstance = this;
}

void pxpk::InputsPC::initInput()
{
	mouseX = 0;
	mouseY = 0;

	glutPassiveMotionFunc(mouseCallback);
	
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(keyboardCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
}

int pxpk::InputsPC::getmouseX()
{
	return mouseX;
}

int pxpk::InputsPC::getmouseY()
{
	return mouseY;
}

bool pxpk::InputsPC::isKeyPressed(unsigned char input)
{
	return keys[input];
}

void pxpk::InputsPC::mouseFunc(int inX, int inY)
{
	mouseX = inX;
	mouseY = inY;
}

void pxpk::InputsPC::keyFunc(unsigned char input)
{
	keys[input] = true;
}

void pxpk::InputsPC::keyUpFunc(unsigned char input)
{
	keys[input] = false;
}

void pxpk::mouseCallback(int inX, int inY)
{
	pxpk::inputsPCInstance->mouseFunc(inX, inY);
}

void pxpk::keyboardCallback(unsigned char inChar, int inX, int inY)
{
	pxpk::inputsPCInstance->keyFunc(inChar);
	pxpk::inputsPCInstance->mouseFunc(inX, inY);
}

void pxpk::keyboardUpCallback(unsigned char inChar, int inX, int inY)
{
	pxpk::inputsPCInstance->keyUpFunc(inChar);
	pxpk::inputsPCInstance->mouseFunc(inX, inY);
}
