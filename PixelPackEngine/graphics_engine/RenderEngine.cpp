#include "RenderEngine.h"

// external function to redirect the render callback
void renderScene()
{
	RenderEngine::getInstance().render();
}

// external GL debug function
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
	Logger::getInstance().log("------------------------------GL ERROR------------------------------", LogLevel::errors);
	Logger::getInstance().log("message: " + messageStr, LogLevel::errors);
	Logger::getInstance().log("type: " + typeStr, LogLevel::errors);
	Logger::getInstance().log("id: " + idStr, LogLevel::errors);
	Logger::getInstance().log("severity: " + severityStr, LogLevel::errors);
	Logger::getInstance().log("------------------------------GL ERROR------------------------------", LogLevel::errors);
}


RenderEngine::RenderEngine()
{
}

void RenderEngine::loadShaders()
{
	//create shaders
	vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//find working directory
	char buf[1000];
	GetModuleFileName(NULL, buf, 1000);
	std::string dir = std::string(buf).substr(0, std::string(buf).find_last_of("\\/"));
	Logger::getInstance().log("working directory: " + dir, LogLevel::info);

	// read vertex shader from file
	std::string vertCode;
	std::ifstream vertStream(dir + "/shaders/vertShader.vert", std::ios::in);
	if (vertStream.is_open())
	{
		vertCode = std::string(std::istreambuf_iterator<char>(vertStream), std::istreambuf_iterator<char>());
		//vertStream >> vertCode;
		vertStream.close();
		Logger::getInstance().log(vertCode, LogLevel::full);
	}
	else
	{
		Logger::getInstance().log("unable to open the vertex shader file", LogLevel::errors);
	}

	// read fragment shader from file
	std::string fragCode;
	std::ifstream fragStream(dir + "/shaders/fragShader.frag", std::ios::in);
	if (fragStream.is_open())
	{
		fragCode = std::string(std::istreambuf_iterator<char>(fragStream), std::istreambuf_iterator<char>());
		//fragStream >> fragCode;
		fragStream.close();
		Logger::getInstance().log(fragCode, LogLevel::full);
	}
	else
	{
		Logger::getInstance().log("unable to open the fragment shader file", LogLevel::errors);
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

		Logger::getInstance().log("Compile failure in vertex shader: " + logStr,LogLevel::errors);
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

		Logger::getInstance().log("Compile failure in fragment shader: " + logStr, LogLevel::errors);
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
		Logger::getInstance().log("Linker error: " + logStr, LogLevel::errors);
		delete[] strInfoLog;
	}

	Logger::getInstance().log("Shaders Loaded", LogLevel::info);
}

void RenderEngine::init(int argc, char **argv, std::string windowName)
{
	//glut setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1280, 720);

	//create window
	glutCreateWindow(windowName.c_str());
	Logger::getInstance().log("Window created", LogLevel::info);

	//glew setup
	GLenum err = glewInit();
	if (err != GLEW_OK)
		Logger::getInstance().log("GLEW initialization failed: " + std::to_string(err), LogLevel::errors);

	//enable features
	glEnable(GL_DEPTH_TEST | GL_DEBUG_OUTPUT_SYNCHRONOUS);

	//register callback functions
	glutDisplayFunc(renderScene);
	glDebugMessageCallback(openglCallback, nullptr);
	GLuint unusedIDs = 0;
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIDs, true);
	Logger::getInstance().log("callbacks registered", LogLevel::info);

	//load the shaders
	loadShaders();

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
	glClearColor(0.0, 0.0, 0.0, 0.0); //clear to black
	//clear the current buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// tell GL to use the shader program
	glUseProgram(programID);

	for (RenderObject i : objects)
	{
		i.draw();
	}

	//swap buffers
	glutSwapBuffers();
}