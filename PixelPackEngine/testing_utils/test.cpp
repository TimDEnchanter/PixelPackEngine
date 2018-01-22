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

	renderer.init(argc, argv, "Test_Window");

	


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
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		0.0, 1.0, 1.0,
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 1.0, 0.0,
		0.0, 1.0, 0.0
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

	//first cube
	int tc1Index = renderer.addObject();
	renderer.setObjVertexBuffer(tc1Index, testVerts);
	renderer.setObjElementBuffer(tc1Index, index);
	renderer.setObjPosition(tc1Index, glm::vec3(0.0, 0.0, 0.0));
	renderer.setObjColor(tc1Index, glm::vec3(0.5, 0.5, 0.5));

	//second cube
	int tc2Index = renderer.addObject();
	renderer.setObjVertexBuffer(tc2Index, testVerts);
	renderer.setObjElementBuffer(tc2Index, index);
	renderer.setObjPosition(tc2Index, glm::vec3(2.0, 0.0, 0.0));
	renderer.setObjColor(tc2Index, glm::vec3(1.0, 0.0, 0.0));

	//third cube
	int tc3Index = renderer.addObject();
	renderer.setObjVertexBuffer(tc3Index, testVerts);
	renderer.setObjElementBuffer(tc3Index, index);
	renderer.setObjPosition(tc3Index, glm::vec3(0.0, 2.0, 0.0));
	renderer.setObjColor(tc3Index, glm::vec3(0.0, 1.0, 0.0));

	//fourth cube
	int tc4Index = renderer.addObject();
	renderer.setObjVertexBuffer(tc4Index, testVerts);
	renderer.setObjElementBuffer(tc4Index, index);
	renderer.setObjPosition(tc4Index, glm::vec3(0.0, 0.0, 2.0));
	renderer.setObjColor(tc4Index, glm::vec3(0.0, 0.0, 1.0));

	//setup camera
	int camIndex = renderer.addCamera();
	renderer.setCamFov(camIndex, 45.0f);
	renderer.setCamNearDist(camIndex, 0.1f);
	renderer.setCamFarDist(camIndex, 100.0f);
	renderer.setCamPosition(camIndex, glm::vec3(10.0f, 10.0f, 10.0f));
	renderer.lookAtCam(camIndex, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0));
	//renderer.setCamOrientationEuler(camIndex, glm::vec3(glm::radians(-35.0), glm::radians(45.0), glm::radians(-0.0)));
	
	renderer.startEngine();

	return 0;
}