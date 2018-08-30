#include "../graphics_engine/RenderEngine.h"
#include "../dependencies/glm/vec3.hpp"
#include "../common/Inputs.h"

/*
test

main function used to test scenarios in the engine
*/

int main(int argc, char **argv)
{
	pxpk::RenderEngine renderer;
	pxpk::Logger::getInstance().setCurrentLogLevel(pxpk::INFO_LOG);

	//renderer.init(argc, argv, "Test_Window");

	//cube vertexes
	/*
	       7--------6
	      /|       /|
	     / |      / |
		3--+-----2  |
		|  4-----+--5
		| /      | /
		|/       |/
		0--------1
	*/

	std::vector<GLfloat> testVerts
	{
		-0.5, -0.5, 0.5,
		0.5, -0.5, 0.5,
		0.5, 0.5, 0.5,
		-0.5, 0.5, 0.5,
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5,
		0.5, 0.5, -0.5,
		-0.5, 0.5, -0.5
	};

	////colors
	//std::vector<GLfloat> testColors
	//{
	//	0.0, 0.0, 1.0,
	//	0.0, 0.0, 1.0,
	//	0.0, 0.0, 1.0,
	//	0.0, 0.0, 1.0,
	//	1.0, 0.0, 0.0,
	//	1.0, 0.0, 0.0,
	//	1.0, 0.0, 0.0,
	//	1.0, 0.0, 0.0
	//};

	//index
	std::vector<GLuint> index 
	{ 
		0,1,2,
		0,2,3,
		0,4,5,
		0,5,1,
		0,7,4,
		0,3,7,
		1,5,6,
		1,6,2,
		3,2,6,
		3,6,7,
		4,6,5,
		4,7,6
	};

	//plane verts
	std::vector<GLfloat> planeVerts
	{
		-100.0, 0.0, 100.0,
		-100.0, 0.0, -100.0,
		100.0, 0.0, -100.0,
		100.0, 0.0, 100.0,
	};

	//plane index
	std::vector<GLuint> planeIndex
	{
		0,1,2,
		2,3,0
	};

	//start render engine
	std::thread renderThread = std::thread(&pxpk::RenderEngine::startEngine, renderer, argc, argv, "Test_Window");
	//renderer.startEngine();

	//lock render queue
	//LOG("Test is locking Render Queue", pxpk::INFO_LOG);
	std::unique_lock<std::mutex> renderLock(pxpk::RenderQ_Mutex);
	//wait for engine to finish startup
	pxpk::RenderQ_Read_CV.wait(renderLock, [] {return pxpk::engineStarted; });
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	pxpk::isRenderWriterReady = false;

	//floor
	unsigned short floorIndex = 0;

	pxpk::RenderQueue::getInstance().objAdd(floorIndex);
	pxpk::RenderQueue::getInstance().objLoadVert(floorIndex, planeVerts);
	pxpk::RenderQueue::getInstance().objLoadIndx(floorIndex, planeIndex);
	pxpk::RenderQueue::getInstance().objSetPos(floorIndex, glm::vec3(0.0, -2.0, 0.0));
	pxpk::RenderQueue::getInstance().objSetColor(floorIndex, glm::vec3(0.2, 0.2, 0.2));

	//first cube
	unsigned short tc1Index = 1;
	
	pxpk::RenderQueue::getInstance().objAdd(tc1Index);
	pxpk::RenderQueue::getInstance().objLoadVert(tc1Index, testVerts);
	pxpk::RenderQueue::getInstance().objLoadIndx(tc1Index, index);
	pxpk::RenderQueue::getInstance().objSetPos(tc1Index, glm::vec3(0.0, 0.0, 0.0));
	pxpk::RenderQueue::getInstance().objSetColor(tc1Index, glm::vec3(0.5, 0.5, 0.5));

	LOG("Cube 1 queued", pxpk::INFO_LOG);

	//second cube
	unsigned short tc2Index = 2;
	pxpk::RenderQueue::getInstance().objAdd(tc2Index);
	pxpk::RenderQueue::getInstance().objLoadVert(tc2Index, testVerts);
	pxpk::RenderQueue::getInstance().objLoadIndx(tc2Index, index);
	pxpk::RenderQueue::getInstance().objSetPos(tc2Index, glm::vec3(2.0, 0.0, 0.0));
	pxpk::RenderQueue::getInstance().objSetColor(tc2Index, glm::vec3(1.0, 0.0, 0.0));

	LOG("Cube 2 queued", pxpk::INFO_LOG);


	//third cube
	unsigned short tc3Index = 3;
	pxpk::RenderQueue::getInstance().objAdd(tc3Index);
	pxpk::RenderQueue::getInstance().objLoadVert(tc3Index, testVerts);
	pxpk::RenderQueue::getInstance().objLoadIndx(tc3Index, index);
	pxpk::RenderQueue::getInstance().objSetPos(tc3Index, glm::vec3(0.0, 2.0, 0.0));
	pxpk::RenderQueue::getInstance().objSetColor(tc3Index, glm::vec3(0.0, 1.0, 0.0));

	LOG("Cube 3 queued", pxpk::INFO_LOG);

	//fourth cube
	unsigned short tc4Index = 4;
	pxpk::RenderQueue::getInstance().objAdd(tc4Index);
	pxpk::RenderQueue::getInstance().objLoadVert(tc4Index, testVerts);
	pxpk::RenderQueue::getInstance().objLoadIndx(tc4Index, index);
	pxpk::RenderQueue::getInstance().objSetPos(tc4Index, glm::vec3(0.0, 0.0, 2.0));
	pxpk::RenderQueue::getInstance().objSetColor(tc4Index, glm::vec3(0.0, 0.0, 1.0));

	LOG("Cube 4 queued", pxpk::INFO_LOG);

	//setup camera
	int camIndex = 0;
	pxpk::RenderQueue::getInstance().camAdd(camIndex);
	pxpk::RenderQueue::getInstance().camSetFov(camIndex, 45.0f);
	pxpk::RenderQueue::getInstance().camSetNear(camIndex, 0.1f);
	pxpk::RenderQueue::getInstance().camSetFar(camIndex, 100.0f);
	pxpk::RenderQueue::getInstance().camSetPos(camIndex, glm::vec3(10.0, 10.0, 10.0));
	pxpk::RenderQueue::getInstance().camLookat(camIndex, glm::vec3(0.0, 0.0, 0.0));

	//renderer.startEngine();

	//manually unlock render queue and signal reader
	//pxpk::Logger::getInstance().log("Test is done with Render Queue", pxpk::INFO_LOG);
	pxpk::isRenderWriterReady = true;
	renderLock.unlock();
	pxpk::RenderQ_Write_CV.notify_all();

	//initial draw
	LOG("populating draw queue", pxpk::INFO_LOG);
	//lock draw queue
	//LOG("Test is locking Draw Queue", pxpk::INFO_LOG);
	std::unique_lock<std::mutex> drawLock(pxpk::DrawQ_Mutex);
	//pxpk::DrawQ_Read_CV.wait(drawLock);

	pxpk::isDrawWriterReady = false;
	pxpk::DrawQueue::getInstance().draw(floorIndex);
	pxpk::DrawQueue::getInstance().draw(tc1Index);
	pxpk::DrawQueue::getInstance().draw(tc2Index);
	pxpk::DrawQueue::getInstance().draw(tc3Index);
	pxpk::DrawQueue::getInstance().draw(tc4Index);

	//manually unlock draw queue and signal reader
	//pxpk::Logger::getInstance().log("Test is done with Draw Queue", pxpk::INFO_LOG);
	pxpk::isDrawWriterReady = true;
	drawLock.unlock();
	pxpk::DrawQ_Write_CV.notify_all();

	LOG("Main loop", pxpk::INFO_LOG);

	pxpk::DeltaTimer deltaTimer = pxpk::DeltaTimer();

	float mouseAngleX = 0.0;
	float mouseAngleY = 0.0;
	glm::vec3 camPos = glm::vec3(10.0, 10.0, 10.0);
	bool lockMouse = true;
	glm::vec3 lookDir = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 rightDir = glm::vec3(0.0, 0.0, 1.0);

	//main loop
	while(true)
	{
		//delta timer
		deltaTimer.tickCheckUpdate();
		float dt = deltaTimer.getFrameTime()/1000.0f;

		//check if time to quit
		if (pxpk::InputsPC::getInstance().isKeyPressed(27))
			exit(0);

		//wait until reader declares render queue is ready
		//LOG("Test is waiting for Render Queue", pxpk::INFO_LOG);
		renderLock.lock();
		pxpk::RenderQ_Read_CV.wait(renderLock, [] {return !pxpk::isRenderWriterReady; });

		//simulate activity
		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
		float rotSpeed = dt * 1.0f;
		pxpk::RenderQueue::getInstance().objRotEuler(2, glm::vec3(rotSpeed, 0.0, 0.0));
		pxpk::RenderQueue::getInstance().objRotEuler(3, glm::vec3(0.0, rotSpeed, 0.0));
		pxpk::RenderQueue::getInstance().objRotEuler(4, glm::vec3(0.0, 0.0, rotSpeed));

		//mouse look
		int winWidth = glutGet(GLUT_WINDOW_WIDTH);
		int winHeight = glutGet(GLUT_WINDOW_HEIGHT);

		if (pxpk::InputsPC::getInstance().isKeyPressed('q'))
			lockMouse = false;
		else
			lockMouse = true;
		if (lockMouse)
		{
			float lookSpeed = 0.8f;
			mouseAngleX += lookSpeed * dt * float(winWidth / 2 - pxpk::InputsPC::getInstance().getmouseX());
			mouseAngleY += lookSpeed * dt * float(winHeight / 2 - pxpk::InputsPC::getInstance().getmouseY());
			lookDir = glm::vec3(
				cos(mouseAngleY) * sin(mouseAngleX),
				sin(mouseAngleY),
				cos(mouseAngleY) * cos(mouseAngleX)
			);
			rightDir = glm::vec3(
				sin(mouseAngleX - 3.14f / 2.0f),
				0,
				cos(mouseAngleX - 3.14f / 2.0f)
			);
			glutWarpPointer(winWidth / 2, winHeight / 2);  //reset mouse to center
		}

		//keyboard movement
		float moveSpeed = 5.0;
		if (pxpk::InputsPC::getInstance().isKeyPressed('w'))
			camPos += lookDir * dt * moveSpeed;
		if (pxpk::InputsPC::getInstance().isKeyPressed('s'))
			camPos -= lookDir * dt * moveSpeed;
		if (pxpk::InputsPC::getInstance().isKeyPressed('d'))
			camPos += rightDir * dt * moveSpeed;
		if (pxpk::InputsPC::getInstance().isKeyPressed('a'))
			camPos -= rightDir * dt * moveSpeed;

		//set movement & look results ORDER IMPORTANT
		pxpk::RenderQueue::getInstance().camSetPos(camIndex, camPos);
		pxpk::RenderQueue::getInstance().camLookat(camIndex, camPos + lookDir);

		//manually unlock render queue and signal reader
		//LOG("Test is done with Render Queue", pxpk::INFO_LOG);
		pxpk::isRenderWriterReady = true;
		renderLock.unlock();
		pxpk::RenderQ_Write_CV.notify_all();

		//wait until reader declares draw queue is ready
		//LOG("Test is waiting for Draw Queue", pxpk::INFO_LOG);
		drawLock.lock();
		pxpk::DrawQ_Read_CV.wait(drawLock, [] {return !pxpk::isDrawWriterReady; });

		//determine what is drawn
		pxpk::DrawQueue::getInstance().draw(floorIndex);
		pxpk::DrawQueue::getInstance().draw(tc1Index);
		pxpk::DrawQueue::getInstance().draw(tc2Index);
		pxpk::DrawQueue::getInstance().draw(tc3Index);
		pxpk::DrawQueue::getInstance().draw(tc4Index);

		//manually unlock draw queue and signal reader
		//LOG("Test is done with Draw Queue", pxpk::INFO_LOG);
		pxpk::isDrawWriterReady = true;
		drawLock.unlock();
		pxpk::DrawQ_Write_CV.notify_all();
	}

	return 0;
}