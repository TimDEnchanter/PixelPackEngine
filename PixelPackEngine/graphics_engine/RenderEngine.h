#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include "SharedVariables.h"
#include "RenderObject.h"
#include "../utility/debugging/Logger.h"
#include "../dependencies/glm/mat4x4.hpp"
#include "../dependencies/glm/gtc/matrix_transform.hpp"

#include <string>
#include <vector>
#include <fstream>

/*
RenderEngine

handles main rendernig operation
*/

class RenderEngine
{
	private:
		GLuint vertexbuffer;


		std::vector<RenderObject> objects;

		GLuint programID;
		GLuint vertShaderID;
		GLuint fragShaderID;

		void loadShaders();

	public:
		/*
		static RenderEngine& getInstance()
		{
			static RenderEngine instance;
			return instance;
		}
		*/
		RenderEngine();
		~RenderEngine();

		void init(int argc, char **argv, std::string windowName);
		void startEngine();

		void addObject(RenderObject);

		void render();
		static void renderCallback();
};

namespace Render {
	namespace {
		//constant instance pointer
		RenderEngine *renderEngineInstance = NULL;
	}
}


#endif // !RENDER_ENGINE_H