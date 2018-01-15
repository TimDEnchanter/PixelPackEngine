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
namespace pxpk {

	class RenderEngine
	{
		private:
			std::vector<pxpk::RenderObject> objects;
			std::vector<pxpk::Camera> cameras;

			GLuint activeCam = 0;

			GLuint programID;
			GLuint vertShaderID;
			GLuint fragShaderID;

			void loadShaders();

		public:
			RenderEngine();
			~RenderEngine();

			GLuint getActiveCam();

			int addObject(pxpk::RenderObject);
			int addCamera(pxpk::Camera);
			void removeObject(int);
			void removeCamera(int);
			void clearObjects();
			void clearCameras();
			
			pxpk::RenderObject & getObject(int);
			pxpk::Camera & getCamera(int);

			void setActiveCam(GLuint);

			void init(int argc, char **argv, std::string windowName);
			void startEngine();

			void render();
			static void renderCallback();
	};


	namespace {
		//constant instance pointer
		RenderEngine *renderEngineInstance = NULL;
	}
}


#endif // !RENDER_ENGINE_H