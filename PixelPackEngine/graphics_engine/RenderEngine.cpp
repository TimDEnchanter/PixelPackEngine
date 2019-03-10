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

void pxpk::RenderEngine::processEvent(pxpk::QueueEvent event)
{
	std::uint8_t cmd = event.getType();
	std::uint16_t ID = event.getID();

	switch (cmd)
	{
	case pxpk::RENDER_OBJ_ADD:
	{
		models.insert({ ID, pxpk::Model() });
		models[ID].setShaderPtr(defaultShader);
		break;
	}
	case pxpk::RENDER_OBJ_REMOVE:
	{
		models.erase(ID);
		break;
	}
	case pxpk::RENDER_OBJ_CLEAR:
	{
		models.clear();
		break;
	}
	case pxpk::RENDER_OBJ_SET_COLOR:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		models[ID].setBaseColor(payload);
		break;
	}
	case pxpk::RENDER_OBJ_SET_MESH:
	{
		std::vector<char> payload;
		event.readPayload(payload);
		std::string file(payload.begin(),payload.end());
		models[ID].setMeshPtr(resources.addMesh(file));
		break;
	}
	case pxpk::RENDER_OBJ_SET_TEX:
	{
		std::vector<char> payload;
		event.readPayload(payload);
		std::string file(payload.begin(), payload.end());
		models[ID].setTexturePtr(resources.addTexture(file));
		break;
	}
	case pxpk::REDEDR_OBJ_SET_SHADER:
	{
		std::vector<char> payload;
		event.readPayload(payload);
		std::string file(payload.begin(), payload.end());
		models[ID].setShaderPtr(resources.addShader(file));
		break;
	}
	case pxpk::RENDER_OBJ_SET_POS:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		models[ID].setPosition(payload);
		break;
	}
	case pxpk::RENDER_OBJ_SET_ORIENT:
	{
		glm::quat payload;
		event.readPayload(payload);
		models[ID].setOrientation(payload);
		break;
	}
	case pxpk::RENDER_OBJ_SET_ORIENT_EULER:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		models[ID].setOrientationEuler(payload);
		break;
	}
	case pxpk::RENDER_OBJ_SET_SCALE:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		models[ID].setScale(payload);
		break;
	}
	case pxpk::RENDER_OBJ_TRANSLATE:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		models[ID].translate(payload);
		break;
	}
	case pxpk::RENDER_OBJ_ROTATE:
	{
		glm::quat payload;
		event.readPayload(payload);
		models[ID].rotate(payload);
		break;
	}
	case pxpk::RENDER_OBJ_ROTATE_EULER:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		models[ID].rotateEuler(payload);
		break;
	}
	case pxpk::RENDER_OBJ_LOOKAT:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		models[ID].lookAt(payload);
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

pxpk::Model & pxpk::RenderEngine::getModel(unsigned short id)
{
	return models[id];
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
		// load default shaders on first run
		defaultShader = resources.addShader("/shaders/default.vert|/shaders/default.frag");

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
	LOG_GL();

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
		LOG_GL();

		pxpk::RenderQueue::getInstance().pop();
	}

	glm::mat4 Projection = cameras[activeCam].getProjectionMatrix();
	glm::mat4 View = cameras[activeCam].getViewMatrix();

	//pxpk::Logger::getInstance().log("drawing objects", pxpk::INFO_LOG);

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

		//fetch shader used for this model
		std::shared_ptr<pxpk::ShaderObject> shaderPtr = models[ID].getShaderPtr();

		//set active shader
		shaderPtr->use();

		//set camera's projection and view
		shaderPtr->setMat4("Projection", Projection);
		shaderPtr->setMat4("View", View);

		//error check
		LOG_GL();

		//draw the model
		models[ID].draw();

		//error check
		LOG_GL();

		pxpk::DrawQueue::getInstance().pop();
	}

	//LOG("swapping buffer", pxpk::INFO_LOG);

	//swap buffers
	glutSwapBuffers();
}