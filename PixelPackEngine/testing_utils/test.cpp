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
	testCube.setDrawMode(GL_QUADS);
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

	std::vector<glm::vec3> testVerts = std::vector<glm::vec3>();
	testVerts.push_back(glm::vec3(0.0, 0.0, 0.0));
	testVerts.push_back(glm::vec3(1.0, 0.0, 0.0));
	testVerts.push_back(glm::vec3(1.0, 1.0, 0.0));
	testVerts.push_back(glm::vec3(0.0, 1.0, 0.0));
	testVerts.push_back(glm::vec3(0.0, 0.0, 1.0));
	testVerts.push_back(glm::vec3(0.0, 0.0, 1.0));
	testVerts.push_back(glm::vec3(0.0, 1.0, 1.0));
	testVerts.push_back(glm::vec3(0.0, 1.0, 1.0));


	testCube.setVertexVector(testVerts);

	//index
	std::vector<GLuint> index 
	{ 1,2,3,4,
	  1,2,6,5,
	  1,4,8,5,
	  2,3,7,6,
	  3,4,8,7,
	  5,6,7,8 };
	testCube.setIndexVector(index);

	testCube.init();
	

	renderer.addObject(testCube);
	

	renderer.startEngine();

	//system("pause");

	return 0;
}