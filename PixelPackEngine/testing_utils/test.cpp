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

	testCube.setIsHidden(false);
	testCube.setDrawMode(GL_TRIANGLES);
	//float colorArray[] = { 1.0, 0.0, 0.0, 1.0 };
	//glm::vec3 testColor(colorArray);
	//testCube.setObjColor(testColor);


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

	//colors
	std::vector<GLfloat> testColors
	{
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0
	};
	testCube.setColorVector(testColors);

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

	testCube.init();
	

	renderer.addObject(testCube);
	

	renderer.startEngine();

	//system("pause");

	return 0;
}