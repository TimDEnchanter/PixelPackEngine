#include "RenderEngine.h"

// external function to redirect the render callback
void renderScene()
{
	RenderEngine::getInstance().render();
}


RenderEngine::RenderEngine()
{
}

void RenderEngine::init(int argc, char **argv, std::string windowName)
{
	//main setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	//create window
	glutCreateWindow(windowName.c_str());

	//enable features
	glEnable(GL_DEPTH_TEST);

	//register callback function
	glutDisplayFunc(renderScene);

	//start main loop
	glutMainLoop();
}


RenderEngine::~RenderEngine()
{
}

void RenderEngine::render()
{
	//clear the current buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 0.0, 0.0, 1.0); //clear to black

	//swap buffers
	glutSwapBuffers();
}