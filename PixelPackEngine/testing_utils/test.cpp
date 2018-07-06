#include "../graphics_engine/RenderEngine.h"
#include "../dependencies/glm/vec3.hpp"

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
		0,3,2,
		0,4,5,
		0,1,5,
		0,4,7,
		0,3,7,
		1,5,6,
		1,2,6,
		3,2,6,
		3,7,6,
		4,5,6,
		4,7,6
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

	//main loop
	while(true)
	{
		//wait until reader declares render queue is ready
		//LOG("Test is waiting for Render Queue", pxpk::INFO_LOG);
		renderLock.lock();
		pxpk::RenderQ_Read_CV.wait(renderLock, [] {return !pxpk::isRenderWriterReady; });

		//simulate activity
		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
		pxpk::RenderQueue::getInstance().objRotEuler(2, glm::vec3(0.005, 0.0, 0.0));
		pxpk::RenderQueue::getInstance().objRotEuler(3, glm::vec3(0.0, 0.005, 0.0));
		pxpk::RenderQueue::getInstance().objRotEuler(4, glm::vec3(0.0, 0.0, 0.005));

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