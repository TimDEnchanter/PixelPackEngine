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

	
	pxpk::RenderObject testCube = pxpk::RenderObject();


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
	testCube.setVertexVector(testVerts);

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
	//testCube.setColorVector(testColors);

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
	testCube.setIndexVector(index);

	//second cube
	pxpk::RenderObject testCube2 = pxpk::RenderObject(testCube);
	testCube2.setPosition(glm::vec3(2.0, 0.0, 0.0));
	//testCube2.setScale(glm::vec3(1.5));
	//testCube2.setOrientationEuler(glm::vec3(0.0, 45.0, 0.0));
	testCube2.setObjColor(glm::vec3(1.0, 0.0, 0.0));

	//third cube
	pxpk::RenderObject testCube3 = pxpk::RenderObject(testCube);
	testCube3.setPosition(glm::vec3(0.0, 2.0, 0.0));
	//testCube3.setScale(glm::vec3(1.5));
	//testCube3.setOrientationEuler(glm::vec3(0.0, 45.0, 0.0));
	testCube3.setObjColor(glm::vec3(0.0, 1.0, 0.0));

	//fourth cube
	pxpk::RenderObject testCube4 = pxpk::RenderObject(testCube);
	testCube4.setPosition(glm::vec3(0.0, 0.0, 2.0));
	//testCube4.setScale(glm::vec3(1.5));
	//testCube4.setOrientationEuler(glm::vec3(0.0, 45.0, 0.0));
	testCube4.setObjColor(glm::vec3(0.0, 0.0, 1.0));


	//init and add cubes
	renderer.addObject(testCube);
	renderer.addObject(testCube2);
	renderer.addObject(testCube3);
	renderer.addObject(testCube4);

	//setup camera
	pxpk::Camera cam = pxpk::Camera();
	cam.setFov(45.0);
	cam.setNearDist(0.1);
	cam.setFarDist(100.0);
	cam.setPosition(glm::vec3(10.0, 10.0, 10.0));
	cam.lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0,1.0,0.0));
	//cam.setOrientationEuler(glm::vec3(glm::radians(-35.0), glm::radians(45.0), glm::radians(-0.0)));
	renderer.addCamera(cam);
	
	renderer.startEngine();

	return 0;
}