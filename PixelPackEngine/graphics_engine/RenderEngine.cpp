#include "RenderEngine.h"

// external function to redirect the render callback
void renderScene()
{
	RenderEngine::getInstance().render();
}


RenderEngine::RenderEngine()
{
}

void RenderEngine::loadShaders()
{
	//create shaders
	GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// read vertex shader from file
	std::string vertCode;
	std::ifstream vertStream("./shaders/vertShader.vert", std::ios::in);
	if (vertStream.is_open())
	{
		vertStream >> vertCode;
		vertStream.close();
	}
	else
	{
		Logger::getInstance().log("unable to open the vertex shader.", LogLevel::errors);
	}

	// read fragment shader from file
	std::string fragCode;
	std::ifstream fragStream("./shaders/fragShader.frag", std::ios::in);
	if (fragStream.is_open())
	{
		fragStream >> fragCode;
		fragStream.close();
	}
	else
	{
		Logger::getInstance().log("unable to open the fragment shader.", LogLevel::errors);
	}

	// compile vertex shader
	const char* vertPointer = vertCode.c_str();
	glShaderSource(vertShaderID, 1, &vertPointer, NULL);
	glCompileShader(vertShaderID);
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

void RenderEngine::addObject(RenderObject input)
{
	objects.push_back(input);
}


RenderEngine::~RenderEngine()
{
}

void RenderEngine::render()
{
	//clear the current buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5, 0.5, 0.5, 1.0); //clear to black

	for (RenderObject i : objects)
	{
		i.draw();
	}

	//swap buffers
	glutSwapBuffers();
}