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
	float colorArray[] = { 1.0, 0.0, 0.0, 1.0 };
	Color testColor = Color(colorArray);
	testCube.setObjColor(testColor);


	//cube vertexes
	/*
	float point1[] = { 0.0, 0.0, 0.0 };
	float point2[] = { 1.0, 0.0, 0.0 };
	float point3[] = { 1.0, 1.0, 0.0 };
	float point4[] = { 0.0, 1.0, 0.0 };

	float point5[] = { 0.0, 0.0, 1.0 };
	float point6[] = { 1.0, 0.0, 1.0 };
	float point7[] = { 1.0, 1.0, 1.0 };
	float point8[] = { 0.0, 1.0, 1.0 };

	std::vector<TriFloat> testVerts = std::vector<TriFloat>();
	testVerts.push_back(TriFloat(point1));
	testVerts.push_back(TriFloat(point2));
	testVerts.push_back(TriFloat(point3));
	testVerts.push_back(TriFloat(point4));
	testVerts.push_back(TriFloat(point5));
	testVerts.push_back(TriFloat(point6));
	testVerts.push_back(TriFloat(point7));
	testVerts.push_back(TriFloat(point8));
	*/

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

	GLfloat testArr[] = {
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		0.0, 1.0, 1.0,
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 1.0, 0.0,
		0.0, 1.0, 0.0
	};
	std::vector<GLfloat> testVerts(testArr, testArr + sizeof testArr / sizeof testArr[0]);


	testCube.setVertexVector(testVerts);

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