#include "RenderEngine.h"

// callback function to redirect the render callback

void pxpk::RenderEngine::renderCallback()
{
	pxpk::renderEngineInstance->render();
}


// external callback GL debug function
void APIENTRY openglCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	//obtain data
	std::string messageStr = message;
	std::string typeStr = "UNKNOWN";
	std::string idStr = std::to_string(id);
	std::string severityStr = "UNKNOWN";

	//set type
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		typeStr = "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		typeStr = "DEPRECEATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		typeStr = "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		typeStr = "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		typeStr = "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		typeStr = "OTHER";
		break;
	}

	//set severity
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_LOW:
		severityStr = "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		severityStr = "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		severityStr = "HIGH";
		break;
	}

	//print logs
	pxpk::Logger::getInstance().log("------------------------------GL ERROR------------------------------", pxpk::ERROR_LOG);
	pxpk::Logger::getInstance().log("message: " + messageStr, pxpk::ERROR_LOG);
	pxpk::Logger::getInstance().log("type: " + typeStr, pxpk::ERROR_LOG);
	pxpk::Logger::getInstance().log("id: " + idStr, pxpk::ERROR_LOG);
	pxpk::Logger::getInstance().log("severity: " + severityStr, pxpk::ERROR_LOG);
	pxpk::Logger::getInstance().log("------------------------------GL ERROR------------------------------", pxpk::ERROR_LOG);
}


pxpk::RenderEngine::RenderEngine()
{
	pxpk::renderEngineInstance = this;
}

void pxpk::RenderEngine::loadShaders()
{
	//create shaders
	vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//find working directory
	char buf[1000];
	GetModuleFileName(NULL, buf, 1000);
	std::string dir = std::string(buf).substr(0, std::string(buf).find_last_of("\\/"));
	pxpk::Logger::getInstance().log("shader directory: " + dir, pxpk::ERROR_LOG);

	// read vertex shader from file
	std::string vertCode;
	std::ifstream vertStream(dir + "/shaders/vertShader.vert", std::ios::in);
	if (vertStream.is_open())
	{
		vertCode = std::string(std::istreambuf_iterator<char>(vertStream), std::istreambuf_iterator<char>());
		vertStream.close();
		//pxpk::Logger::getInstance().log(vertCode, pxpk::ERROR_LOG);
	}
	else
	{
		pxpk::Logger::getInstance().log("unable to open the vertex shader file", pxpk::ERROR_LOG);
	}

	// read fragment shader from file
	std::string fragCode;
	std::ifstream fragStream(dir + "/shaders/fragShader.frag", std::ios::in);
	if (fragStream.is_open())
	{
		fragCode = std::string(std::istreambuf_iterator<char>(fragStream), std::istreambuf_iterator<char>());
		fragStream.close();
		//pxpk::Logger::getInstance().log(fragCode, pxpk::ERROR_LOG);
	}
	else
	{
		pxpk::Logger::getInstance().log("unable to open the fragment shader file", pxpk::ERROR_LOG);
	}

	// compile vertex shader
	const char* vertPointer = vertCode.c_str();
	glShaderSource(vertShaderID, 1, &vertPointer, NULL);
	glCompileShader(vertShaderID);
	GLint vertStatus;
	glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &vertStatus);
	if (vertStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(vertShaderID, infoLogLength, NULL, strInfoLog);

		std::string logStr = strInfoLog;

		pxpk::Logger::getInstance().log("Compile failure in vertex shader: " + logStr, pxpk::ERROR_LOG);
		delete[] strInfoLog;
	}

	// compile fragment shader
	const char* fragPointer = fragCode.c_str();
	glShaderSource(fragShaderID, 1, &fragPointer, NULL);
	glCompileShader(fragShaderID);
	GLint fragStatus;
	glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &fragStatus);
	if (fragStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(fragShaderID, infoLogLength, NULL, strInfoLog);

		std::string logStr = strInfoLog;

		pxpk::Logger::getInstance().log("Compile failure in fragment shader: " + logStr, pxpk::ERROR_LOG);
		delete[] strInfoLog;
	}

	// add shaders to program
	programID = glCreateProgram();
	glAttachShader(programID, vertShaderID);
	glAttachShader(programID, fragShaderID);

	// link program to GL
	glLinkProgram(programID);
	GLint status;
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(programID, infoLogLength, NULL, strInfoLog);
		std::string logStr = strInfoLog;
		pxpk::Logger::getInstance().log("Linker error: " + logStr, pxpk::ERROR_LOG);
		delete[] strInfoLog;
	}

	glDetachShader(programID, vertShaderID);
	glDetachShader(programID, fragShaderID);

	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);

	pxpk::Logger::getInstance().log("Shaders Loaded", pxpk::INFO_LOG);
}

void pxpk::RenderEngine::init(int argc, char **argv, std::string windowName)
{
	//glut setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(pxpk::windowWidth, pxpk::windowHeight);

	//create window
	glutCreateWindow(windowName.c_str());
	pxpk::Logger::getInstance().log("Window created", pxpk::INFO_LOG);

	//glew setup
	GLenum err = glewInit();
	if (err != GLEW_OK)
		pxpk::Logger::getInstance().log("GLEW initialization failed: " + std::to_string(err), pxpk::ERROR_LOG);

	//enable features
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	//set function to determine depth culling
	glDepthFunc(GL_LESS);

	//register callback functions
	glutDisplayFunc(renderCallback);
	glDebugMessageCallback(openglCallback, nullptr);
	GLuint unusedIDs = 0;
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIDs, true);
	pxpk::Logger::getInstance().log("callbacks registered", pxpk::INFO_LOG);

	//load the shaders
	loadShaders();
}

void pxpk::RenderEngine::startEngine()
{
	//start main loop
	glutMainLoop();
}

void pxpk::RenderEngine::addObject(pxpk::RenderObject input)
{
	pxpk::Logger::getInstance().log("adding object", pxpk::INFO_LOG);
	input.init();
	objects.push_back(input);
}

void pxpk::RenderEngine::addCamera(Camera input)
{
	cameras.push_back(input);
}

void pxpk::RenderEngine::setActiveCam(GLuint input)
{
	activeCam = input;
}


pxpk::RenderEngine::~RenderEngine()
{
}

std::vector<pxpk::RenderObject> pxpk::RenderEngine::getObjects()
{
	return objects;
}

std::vector<pxpk::Camera> pxpk::RenderEngine::getCameras()
{
	return cameras;
}

GLuint pxpk::RenderEngine::getActiveCam()
{
	return activeCam;
}

void pxpk::RenderEngine::render()
{
	//pxpk::Logger::getInstance().log("clearing buffer", pxpk::INFO_LOG);

	glClearColor(0.0, 0.0, 0.0, 0.0); //clear to black
	//clear the current buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)pxpk::windowWidth / (float)pxpk::windowHeight, 0.1f, 100.0f);
	glm::mat4 Projection = cameras[activeCam].getProjectionMatrix();

	//glm::mat4 View = glm::lookAt(
	//	glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
	//	glm::vec3(0, 0, 0), // and looks at the origin
	//	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	//);
	glm::mat4 View = cameras[activeCam].getViewMatrix();

	GLuint mvpID = glGetUniformLocation(programID, "MVP");

	//pxpk::Logger::getInstance().log("drawing objects", pxpk::INFO_LOG);

	// tell GL to use the shader program
	glUseProgram(programID);
	
	for (pxpk::RenderObject i : objects)
	{
		// get model matrix from object
		glm::mat4 Model = i.getModelMatrix();

		// calculate MVP and send to shader
		glm::mat4 mvp = Projection * View * Model;
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

		i.draw();
	}
	

	//pxpk::Logger::getInstance().log("swapping buffer", pxpk::INFO_LOG);

	//swap buffers
	glutSwapBuffers();
}