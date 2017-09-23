#include "../graphics_engine/RenderEngine.h"

/*
test

main function used to test scenarios in the engine
*/

int main(int argc, char **argv)
{
	RenderEngine renderer = RenderEngine::getInstance();

	renderer.init(argc, argv, "Test_Window");

	//system("pause");

	return 0;
}