#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include "..\dependencies\glew\glew.h"
#include "..\dependencies\freeglut\freeglut.h"
#include <string>

/*
RenderEngine

singleton class that handles main rendernig operation
*/

class RenderEngine
{
	private:
		RenderEngine();


	public:
		static RenderEngine& getInstance()
		{
			static RenderEngine instance;
			return instance;
		}

		~RenderEngine();

		void init(int argc, char **argv, std::string windowName);

		void render();
};

#endif // !RENDER_ENGINE_H