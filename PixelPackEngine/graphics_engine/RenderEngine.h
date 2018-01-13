#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include "SharedVariables.h"
#include "RenderObject.h"
#include "Camera.h"
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
		std::vector<RenderObject> objects;
		std::vector<Camera> cameras;

		GLuint activeCam = 0;

		GLuint programID;
		GLuint vertShaderID;
		GLuint fragShaderID;

		void loadShaders();

	public:
		RenderEngine();
		~RenderEngine();

		std::vector<RenderObject> getObjects();
		std::vector<Camera> getCameras();
		GLuint getActiveCam();

		void addObject(RenderObject);
		void addCamera(Camera);
		void setActiveCam(GLuint);

		void init(int argc, char **argv, std::string windowName);
		void startEngine();

		void render();
		static void renderCallback();
};

namespace pxpk {
	namespace {
		//constant instance pointer
		RenderEngine *renderEngineInstance = NULL;
	}
}


#endif // !RENDER_ENGINE_H