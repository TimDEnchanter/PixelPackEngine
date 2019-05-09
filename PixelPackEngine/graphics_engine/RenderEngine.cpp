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
		models[ID].setTexturePtr(defaultTexture);
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
	case pxpk::RENDER_OBJ_SET_AMBI:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		models[ID].setAmbient(payload);
		break;
	}
	case pxpk::RENDER_OBJ_SET_DIFF:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		models[ID].setDiffuse(payload);
		break;
	}
	case pxpk::RENDER_OBJ_SET_SPEC:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		models[ID].setSpecular(payload);
		break;
	}
	case pxpk::RENDER_OBJ_SET_SHINE:
	{
		GLfloat payload;
		event.readPayload(payload);
		models[ID].setShininess(payload);
		break;
	}
	case pxpk::RENDER_OBJ_SET_MESH:
	{
		std::vector<char> payload;
		event.readPayload(payload);
		std::string file(payload.begin(),payload.end());
		models[ID].setMeshPtr(meshes.addMesh(file));
		break;
	}
	case pxpk::RENDER_OBJ_SET_TEX:
	{
		std::vector<char> payload;
		event.readPayload(payload);
		std::string file(payload.begin(), payload.end());
		models[ID].setTexturePtr(textures.addTexture(file));
		break;
	}
	case pxpk::RENDER_OBJ_SET_SHADER:
	{
		std::vector<char> payload;
		event.readPayload(payload);
		std::string file(payload.begin(), payload.end());
		models[ID].setShaderPtr(shaders.addShader(file));
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
	case pxpk::RENDER_LIGHT_REMOVE:
	{
		if (pointLights.find(ID) != pointLights.end())
			pointLights.erase(ID);
		else if (dirLights.find(ID) != dirLights.end())
			dirLights.erase(ID);
		else if (spotLights.find(ID) != spotLights.end())
			spotLights.erase(ID);
		break;
	}
	case pxpk::RENDER_LIGHT_CLEAR:
	{
		pointLights.clear();
		dirLights.clear();
		spotLights.clear();
		break;
	}
	case pxpk::RENDER_LIGHT_SET_POS:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		if (pointLights.find(ID) != pointLights.end())
			pointLights[ID].setPosition(payload);
		else if (dirLights.find(ID) != dirLights.end())
			dirLights[ID].setPosition(payload);
		else if (spotLights.find(ID) != spotLights.end())
			spotLights[ID].setPosition(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_SET_ORIENT:
	{
		glm::quat payload;
		event.readPayload(payload);
		if (pointLights.find(ID) != pointLights.end())
			pointLights[ID].setOrientation(payload);
		else if (dirLights.find(ID) != dirLights.end())
			dirLights[ID].setOrientation(payload);
		else if (spotLights.find(ID) != spotLights.end())
			spotLights[ID].setOrientation(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_SET_ORIENT_EULER:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		if (pointLights.find(ID) != pointLights.end())
			pointLights[ID].setOrientationEuler(payload);
		else if (dirLights.find(ID) != dirLights.end())
			dirLights[ID].setOrientationEuler(payload);
		else if (spotLights.find(ID) != spotLights.end())
			spotLights[ID].setOrientationEuler(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_TRANSLATE:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		if (pointLights.find(ID) != pointLights.end())
			pointLights[ID].translate(payload);
		else if (dirLights.find(ID) != dirLights.end())
			dirLights[ID].translate(payload);
		else if (spotLights.find(ID) != spotLights.end())
			spotLights[ID].translate(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_ROTATE:
	{
		glm::quat payload;
		event.readPayload(payload);
		if (pointLights.find(ID) != pointLights.end())
			pointLights[ID].rotate(payload);
		else if (dirLights.find(ID) != dirLights.end())
			dirLights[ID].rotate(payload);
		else if (spotLights.find(ID) != spotLights.end())
			spotLights[ID].rotate(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_ROTATE_EULER:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		if (pointLights.find(ID) != pointLights.end())
			pointLights[ID].rotateEuler(payload);
		else if (dirLights.find(ID) != dirLights.end())
			dirLights[ID].rotateEuler(payload);
		else if (spotLights.find(ID) != spotLights.end())
			spotLights[ID].rotateEuler(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_LOOKAT:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		if (pointLights.find(ID) != pointLights.end())
			pointLights[ID].lookAt(payload);
		else if (dirLights.find(ID) != dirLights.end())
			dirLights[ID].lookAt(payload);
		else if (spotLights.find(ID) != spotLights.end())
			spotLights[ID].lookAt(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_SET_SHADER:
	{
		std::vector<char> payload;
		event.readPayload(payload);
		std::string file(payload.begin(), payload.end());
		if (pointLights.find(ID) != pointLights.end())
			pointLights[ID].setShaderPtr(shaders.addShader(file));
		else if (dirLights.find(ID) != dirLights.end())
			dirLights[ID].setShaderPtr(shaders.addShader(file));
		else if (spotLights.find(ID) != spotLights.end())
			spotLights[ID].setShaderPtr(shaders.addShader(file));
		break;
	}
	case pxpk::RENDER_LIGHT_SET_AMBI:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		if (pointLights.find(ID) != pointLights.end())
			pointLights[ID].setAmbient(payload);
		else if (dirLights.find(ID) != dirLights.end())
			dirLights[ID].setAmbient(payload);
		else if (spotLights.find(ID) != spotLights.end())
			spotLights[ID].setAmbient(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_SET_DIFF:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		if (pointLights.find(ID) != pointLights.end())
			pointLights[ID].setDiffuse(payload);
		else if (dirLights.find(ID) != dirLights.end())
			dirLights[ID].setDiffuse(payload);
		else if (spotLights.find(ID) != spotLights.end())
			spotLights[ID].setDiffuse(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_SET_SPEC:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		if (pointLights.find(ID) != pointLights.end())
			pointLights[ID].setSpecular(payload);
		else if (dirLights.find(ID) != dirLights.end())
			dirLights[ID].setSpecular(payload);
		else if (spotLights.find(ID) != spotLights.end())
			spotLights[ID].setSpecular(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_POINT_ADD:
	{
		pointLights.insert({ ID, pxpk::PointLight() });
		pointLights[ID].setShaderPtr(defaultShader);
		break;
	}
	case pxpk::RENDER_LIGHT_POINT_SET_CONST:
	{
		GLfloat payload;
		event.readPayload(payload);
		pointLights[ID].setConstant(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_POINT_SET_LIN:
	{
		GLfloat payload;
		event.readPayload(payload);
		pointLights[ID].setLinear(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_POINT_SET_QUAD:
	{
		GLfloat payload;
		event.readPayload(payload);
		pointLights[ID].setQuadratic(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_DIR_ADD:
	{
		dirLights.insert({ ID, pxpk::DirLight() });
		dirLights[ID].setShaderPtr(defaultShader);
		break;
	}
	case pxpk::RENDER_LIGHT_DIR_SET_DIR:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		dirLights[ID].setDirection(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_SPOT_ADD:
	{
		spotLights.insert({ ID, pxpk::SpotLight() });
		spotLights[ID].setShaderPtr(defaultShader);
		break;
	}
	case pxpk::RENDER_LIGHT_SPOT_SET_DIR:
	{
		glm::vec3 payload;
		event.readPayload(payload);
		spotLights[ID].setDirection(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_SPOT_SET_IN_CUTOFF:
	{
		GLfloat payload;
		event.readPayload(payload);
		spotLights[ID].setInnerCutoff(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_SPOT_SET_OUT_CUTOFF:
	{
		GLfloat payload;
		event.readPayload(payload);
		spotLights[ID].setOuterCutoff(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_SPOT_SET_CONST:
	{
		GLfloat payload;
		event.readPayload(payload);
		spotLights[ID].setConstant(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_SPOT_SET_LIN:
	{
		GLfloat payload;
		event.readPayload(payload);
		spotLights[ID].setLinear(payload);
		break;
	}
	case pxpk::RENDER_LIGHT_SPOT_SET_QUAD:
	{
		GLfloat payload;
		event.readPayload(payload);
		spotLights[ID].setQuadratic(payload);
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

pxpk::Light & pxpk::RenderEngine::getLight(unsigned short id)
{
	return pointLights[id];
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
		// load default resources on first run
		defaultShader = shaders.addShader("/shaders/default.vert|/shaders/default.frag");
		defaultTexture = textures.addTexture("/textures/default.jpg");

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

	//get Projection and View from camera
	glm::mat4 Projection = cameras[activeCam].getProjectionMatrix();
	glm::mat4 View = cameras[activeCam].getViewMatrix();
	glm::vec3 camPos = cameras[activeCam].getPosition();

	//get size of light containers
	int numPointLights = pointLights.size();
	int numDirLights = dirLights.size();
	int numSpotLights = spotLights.size();

	//assign global uniforms to all shaders
	for (std::pair< std::string, std::weak_ptr<pxpk::ObjectResource> > it : shaders.getMap())
	{
		std::shared_ptr<pxpk::ShaderObject> temp = std::static_pointer_cast<pxpk::ShaderObject>(std::shared_ptr<pxpk::ObjectResource>(it.second));
		temp->use();

		//projection and view
		temp->setMat4("Projection", Projection);
		temp->setMat4("View", View);
		temp->setVec3("viewPos", camPos);

		temp->setInt("numPointLights", numPointLights);
		temp->setInt("numDirLights", numDirLights);
		temp->setInt("numSpotLights", numSpotLights);
	}

	//setup lights
	int i = 0;
	for (std::pair<unsigned short, pxpk::PointLight> it : pointLights)
		it.second.draw(i++);
	i = 0;
	for (std::pair<unsigned short, pxpk::DirLight> it : dirLights)
		it.second.draw(i++);
	i = 0;
	for (std::pair<unsigned short, pxpk::SpotLight> it : spotLights)
		it.second.draw(i++);

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