#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "RenderObject.h"
#include <string>
#include <vector>

/*
RenderEngine

singleton class that handles main rendernig operation
*/

class RenderEngine
{
	private:
		std::vector<RenderObject> objects;

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