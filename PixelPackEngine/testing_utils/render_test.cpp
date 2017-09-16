#include "../graphics_engine/RenderEngine.h"

/*
render_test

main function used to test scenarios in the graphics engine
*/

int main(int argc, char **argv)
{
	RenderEngine renderer = RenderEngine::getInstance();

	renderer.init(argc, argv, "Test_Window");

	return 0;
}