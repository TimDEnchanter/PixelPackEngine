#include "RenderEngine.h"

// implement extern vars
namespace pxpk
{
	bool engineStarted = false;
}

// callback function to redirect the render callback
void pxpk::RenderEngine::renderCallback()
{
	pxpk::renderEngineInstance->render();
}

void pxpk::RenderEngine::idleCallback()
{
	glutPostRedisplay();
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
	LOG("------------------------------GL ERROR------------------------------", pxpk::ERROR_LOG);
	LOG("message: " + messageStr, pxpk::ERROR_LOG);
	LOG("type: " + typeStr, pxpk::ERROR_LOG);
	LOG("id: " + idStr, pxpk::ERROR_LOG);
	LOG("severity: " + severityStr, pxpk::ERROR_LOG);
	LOG("------------------------------GL ERROR------------------------------", pxpk::ERROR_LOG);
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
	LOG("shader directory: " + dir, pxpk::ERROR_LOG);

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
		LOG("unable to open the vertex shader file", pxpk::ERROR_LOG);
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
		LOG("unable to open the fragment shader file", pxpk::ERROR_LOG);
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

		LOG("Compile failure in vertex shader: " + logStr, pxpk::ERROR_LOG);
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

		LOG("Compile failure in fragment shader: " + logStr, pxpk::ERROR_LOG);
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
		LOG("Linker error: " + logStr, pxpk::ERROR_LOG);
		delete[] strInfoLog;
	}

	glDetachShader(programID, vertShaderID);
	glDetachShader(programID, fragShaderID);

	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);

	LOG("Shaders Loaded", pxpk::INFO_LOG);
}

void pxpk::RenderEngine::processEvent(pxpk::QueueEvent event)
{
	std::uint8_t cmd = event.getType();
	std::uint16_t ID = event.getID();

	switch (cmd)
	{
	case pxpk::RENDER_OBJ_ADD:
	{
		objects.insert({ ID, pxpk::RenderObject() });
		break;
	}
	case pxpk::RENDER_OBJ_REMOVE:
	{
		objects.erase(ID);
		break;
	}
	case pxpk::RENDER_OBJ_CLEAR:
	{
		objects.clear();
		break;
	}
	case pxpk::RENDER_OBJ_LOAD_VERT:
	{
		std::vector<GLfloat> payload;
		event.readPayload(payload);
		objects[ID].setVertexVector(payload);
		break;
	}
	case pxpk::RENDER_OBJ_LOAD_INDEX:
	{
		std::vector<GLuint> payload;
		event.readPayload(payload);
		objects[ID].setIndexVector(payload);
		break;
	}
	case pxpk::RENDER_OBJ_LOAD_COLOR:
	{
		std::vector<GLfloat> payload;
		event.readPayload(payload);
		objects[ID].setColorVector(payload);
		break;
	}
	case pxpk::RENDER_OBJ_SET_COLOR:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		objects[ID].setObjColor(payload);
		break;
	}
	case pxpk::RENDER_OBJ_SET_POS:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		objects[ID].setPosition(payload);
		break;
	}
	case pxpk::RENDER_OBJ_SET_ORIENT:
	{
		glm::quat payload;
		event.readPayload(payload);
		objects[ID].setOrientation(payload);
		break;
	}
	case pxpk::RENDER_OBJ_SET_ORIENT_EULER:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		objects[ID].setOrientationEuler(payload);
		break;
	}
	case pxpk::RENDER_OBJ_SET_SCALE:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		objects[ID].setScale(payload);
		break;
	}
	case pxpk::RENDER_OBJ_TRANSLATE:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		objects[ID].translate(payload);
		break;
	}
	case pxpk::RENDER_OBJ_ROTATE:
	{
		glm::quat payload;
		event.readPayload(payload);
		objects[ID].rotate(payload);
		break;
	}
	case pxpk::RENDER_OBJ_ROTATE_EULER:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		objects[ID].rotateEuler(payload);
		break;
	}
	case pxpk::RENDER_OBJ_LOOKAT:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		objects[ID].lookAt(payload);
		break;
	}
	case pxpk::RENDER_CAM_ADD:
	{
		cameras.insert({ ID, pxpk::Camera() });
		break;
	}
	case pxpk::RENDER_CAM_REMOVE:
	{
		cameras.erase(ID);
		break;
	}
	case pxpk::RENDER_CAM_CLEAR:
	{
		cameras.clear();
		break;
	}
	case pxpk::RENDER_CAM_SET_POS:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		cameras[ID].setPosition(payload);
		break;
	}
	case pxpk::RENDER_CAM_SET_ORIENT:
	{
		glm::quat payload;
		event.readPayload(payload);
		cameras[ID].setOrientation(payload);
		break;
	}
	case pxpk::RENDER_CAM_SET_ORIENT_EULER:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		cameras[ID].setOrientationEuler(payload);
		break;
	}
	case pxpk::RENDER_CAM_TRANSLATE:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		cameras[ID].translate(payload);
		break;
	}
	case pxpk::RENDER_CAM_ROTATE:
	{
		glm::quat payload;
		event.readPayload(payload);
		cameras[ID].rotate(payload);
		break;
	}
	case pxpk::RENDER_CAM_ROTATE_EULER:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		cameras[ID].rotateEuler(payload);
		break;
	}
	case pxpk::RENDER_CAM_LOOKAT:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		cameras[ID].lookAt(payload, glm::vec3(0.0f, 1.0f, 0.0f)); //figure out what I want to do with this
		break;
	}
	case pxpk::RENDER_CAM_SET_FOV:
	{
		GLfloat payload;
		event.readPayload(payload);
		cameras[ID].setFov(payload);
		break;
	}
	case pxpk::RENDER_CAM_SET_NEAR:
	{
		GLfloat payload;
		event.readPayload(payload);
		cameras[ID].setNearDist(payload);
		break;
	}
	case pxpk::RENDER_CAM_SET_FAR:
	{
		GLfloat payload;
		event.readPayload(payload);
		cameras[ID].setFarDist(payload);
		break;
	}
	case pxpk::RENDER_CAM_SET_ACTIVE:
	{
		activeCam = ID;
		break;
	}
	}
}

void pxpk::RenderEngine::checkGLError(const char *file, int line)
{
	GLenum err(glGetError());

	while (err != GL_NO_ERROR)
	{
		std::string errStr;

		switch (err)
		{
		case GL_INVALID_OPERATION: errStr = "OpenGL error - INVALID_OPERATION"; break;
		case GL_INVALID_ENUM:  errStr = "OpenGL error - INVALID_ENUM"; break;
		case GL_INVALID_VALUE:  errStr = "OpenGL error - INVALID_VALUE"; break;
		case GL_OUT_OF_MEMORY:  errStr = "OpenGL error - OUT_OF_MEMORY"; break;
		case GL_STACK_OVERFLOW:  errStr = "OpenGL error - STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:  errStr = "OpenGL error - STACK_UNDERFLOW"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  errStr = "OpenGL error - INVALID_FRAMEBUFFER_OPERATION"; break;
		case GL_CONTEXT_LOST:  errStr = "OpenGL error - CONTEXT_LOST"; break;
		case GL_TABLE_TOO_LARGE:  errStr = "OpenGL error - TABLE_TOO_LARGE"; break;
		default: errStr = "OpenGL error - Unknown"; break;
		}

		pxpk::Logger::getInstance()._log(errStr, pxpk::ERROR_LOG, file, line);

		err = glGetError();
	}
}

void pxpk::RenderEngine::init(int argc, char **argv, std::string windowName)
{
	//glut setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(pxpk::windowWidth, pxpk::windowHeight);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE
#if _DEBUG
		| GLUT_DEBUG
#endif
	);

	//create window
	glutCreateWindow(windowName.c_str());
	LOG("Window created", pxpk::INFO_LOG);

	//glew setup
	GLenum err = glewInit();
	if (err != GLEW_OK)
		LOG("GLEW initialization failed: " + std::to_string(err), pxpk::ERROR_LOG);

	//enable features
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_CULL_FACE);

	//set function to determine depth culling
	glDepthFunc(GL_LESS);

	//register callback functions
	glutDisplayFunc(renderCallback);
	glutIdleFunc(idleCallback);
	glDebugMessageCallback(openglCallback, nullptr);
	GLuint unusedIDs = 0;
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIDs, true);
	LOG("callbacks registered", pxpk::INFO_LOG);

	//load the shaders
	//loadShaders();

	//start input sampling
	pxpk::InputsPC::getInstance().initInput();

	//remove mouse cursor display
	glutSetCursor(GLUT_CURSOR_NONE);
}

void pxpk::RenderEngine::startEngine(int argc, char **argv, std::string windowName)
{
	LOG("Starting Render Engine", pxpk::INFO_LOG);
	init(argc, argv, windowName);

	//start main loop
	glutMainLoop();
}

void pxpk::RenderEngine::drawObj(unsigned short id)
{
	objects[id].draw();
}

pxpk::RenderObject & pxpk::RenderEngine::getObject(unsigned short id)
{
	return objects[id];
}

pxpk::Camera & pxpk::RenderEngine::getCamera(unsigned short id)
{
	return cameras[id];
}

pxpk::RenderEngine::~RenderEngine()
{
}

unsigned short pxpk::RenderEngine::getActiveCam()
{
	return activeCam;
}

void pxpk::RenderEngine::render()
{
	//tick for FPS
	if (frameTimer.tickCheckUpdate())
	{
		float frameTime = frameTimer.getFrameTime()/1000.0f;
		//system("cls");
		//LOG("Frametime - " + std::to_string(frameTime), pxpk::INFO_LOG);
		//LOG("FPS - " + std::to_string(1.0/frameTime), pxpk::INFO_LOG);
	}

	if (!pxpk::engineStarted)
	{
		// load shaders on first run
		loadShaders();

		LOG("Engine Started", pxpk::INFO_LOG);
		//signal writer that engine has started
		pxpk::engineStarted = true;
		pxpk::RenderQ_Read_CV.notify_all();
	}
	//LOG("clearing buffer", pxpk::INFO_LOG);

	glClearColor(0.0, 0.0, 0.0, 0.0); //clear to black
	//clear the current buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//error check
	//checkGLError(__FILENAME__, __LINE__);

	//wait until writer declares render queue is ready
	//LOG("Engine is waiting for Render Queue", pxpk::INFO_LOG);
	std::unique_lock<std::mutex> renderLock(pxpk::RenderQ_Mutex);
	pxpk::RenderQ_Write_CV.wait(renderLock, [] {return pxpk::isRenderWriterReady;});

	//swap render buffer
	pxpk::RenderQueue::getInstance().swap();
	pxpk::isRenderWriterReady = false;

	//manually unlock render queue and signal writer
	//LOG("Engine is done with Render Queue", pxpk::INFO_LOG);
	renderLock.unlock();
	pxpk::RenderQ_Read_CV.notify_all();

	//process all commands in the render queue
	while (!pxpk::RenderQueue::getInstance().isReadEmpty())
	{
		QueueEvent event = pxpk::RenderQueue::getInstance().read();
		processEvent(event);
		//error check
		//checkGLError(__FILENAME__, __LINE__);

		pxpk::RenderQueue::getInstance().pop();
	}

	glm::mat4 Projection = cameras[activeCam].getProjectionMatrix();

	glm::mat4 View = cameras[activeCam].getViewMatrix();

	GLuint mvpID = glGetUniformLocation(programID, "MVP");

	//pxpk::Logger::getInstance().log("drawing objects", pxpk::INFO_LOG);

	// tell GL to use the shader program
	glUseProgram(programID);
	
	//error check
	//checkGLError(__FILENAME__, __LINE__);

	//wait unitl writer declares draw queue is ready
	//LOG("Engine is waiting for Draw Queue", pxpk::INFO_LOG);
	std::unique_lock<std::mutex> drawLock(pxpk::DrawQ_Mutex);
	pxpk::DrawQ_Write_CV.wait(drawLock, [] {return pxpk::isDrawWriterReady; });

	//swap draw buffer
	pxpk::DrawQueue::getInstance().swap();
	pxpk::isDrawWriterReady = false;

	//manually unlock draw queue and signal writer
	//LOG("Engine is done with Draw Queue", pxpk::INFO_LOG);
	drawLock.unlock();
	pxpk::DrawQ_Read_CV.notify_all();

	while (!pxpk::DrawQueue::getInstance().isReadEmpty())
	{
		//fetch ID from queue
		unsigned short ID = pxpk::DrawQueue::getInstance().read().getID();

		//get Model matrix from object
		glm::mat4 Model = objects[ID].getModelMatrix();

		//calulate MVP and send to shader
		glm::mat4 mvp = Projection * View * Model;
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

		//draw the object
		objects[ID].draw();

		//error check
		//checkGLError(__FILENAME__, __LINE__);

		pxpk::DrawQueue::getInstance().pop();
	}

	//LOG("swapping buffer", pxpk::INFO_LOG);

	//swap buffers
	glutSwapBuffers();
}