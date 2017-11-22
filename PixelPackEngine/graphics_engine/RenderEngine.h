#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include "SharedVariables.h"
#include "RenderObject.h"
#include "../utility/debugging/Logger.h"

#include <string>
#include <vector>
#include <fstream>

/*
RenderEngine

singleton class that handles main rendernig operation
*/

class RenderEngine
{
	private:
		std::vector<RenderObject> objects;

		RenderEngine();

		void loadShaders();

	public:
		static RenderEngine& getInstance()
		{
			static RenderEngine instance;
			return instance;
		}

		~RenderEngine();

		void init(int argc, char **argv, std::string windowName);

		void addObject(RenderObject);

		void render();
};

#endif // !RENDER_ENGINE_H