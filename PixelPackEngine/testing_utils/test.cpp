#include "../graphics_engine/RenderEngine.h"
#include "../dependencies/glm/vec3.hpp"

/*
test

main function used to test scenarios in the engine
*/

int main(int argc, char **argv)
{
	RenderEngine renderer;
	Logger::getInstance().setCurrentLogLevel(LogLevel::info);

	renderer.init(argc, argv, "Test_Window");

	
	RenderObject testCube = RenderObject();


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
	RenderObject testCube2 = RenderObject(testCube);
	testCube2.setPosition(glm::vec3(2.0, 0.0, 0.0));
	//testCube2.setScale(glm::vec3(1.5));
	//testCube2.setOrientationEuler(glm::vec3(0.0, 45.0, 0.0));
	testCube2.setObjColor(glm::vec3(1.0, 0.0, 0.0));

	//third cube
	RenderObject testCube3 = RenderObject(testCube);
	testCube3.setPosition(glm::vec3(0.0, 2.0, 0.0));
	//testCube3.setScale(glm::vec3(1.5));
	//testCube3.setOrientationEuler(glm::vec3(0.0, 45.0, 0.0));
	testCube3.setObjColor(glm::vec3(0.0, 1.0, 0.0));

	//fourth cube
	RenderObject testCube4 = RenderObject(testCube);
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
	Camera cam = Camera();
	cam.setFov(45.0);
	cam.setNearDist(0.1);
	cam.setFarDist(100.0);
	cam.setPosition(glm::vec3(10.0, 10.0, 10.0));
	cam.setLookPoint(glm::vec3(0.0, 0.0, 0.0));
	//cam.setOrientationEuler(glm::vec3(0.0, 90.0, 0.0));
	renderer.addCamera(cam);
	
	renderer.startEngine();

	return 0;
}