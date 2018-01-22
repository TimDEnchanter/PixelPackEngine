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

			int addObject();
			int addCamera();
			int addObject(pxpk::RenderObject);
			int addCamera(pxpk::Camera);
			void removeObject(int);
			void removeCamera(int);
			void clearObjects();
			void clearCameras();

			void setObjVertexBuffer(int, std::vector<GLfloat>);
			void setObjElementBuffer(int, std::vector<GLuint>);
			void setObjColorBuffer(int, std::vector<GLfloat>);
			void setObjColor(int, glm::vec3);
			void setObjPosition(int, glm::vec3);
			void setObjOrientation(int, glm::quat);
			void setObjOrientationEuler(int, glm::vec3);
			void setObjScale(int, glm::vec3);
			void translateObj(int, glm::vec3);
			void rotateEulerObj(int, glm::vec3);
			void rotateObj(int, glm::quat);
			void lookAtObj(int, glm::vec3);
			void drawObj(int);

			void setCamPosition(int, glm::vec3);
			void setCamOrientation(int, glm::quat);
			void setCamOrientationEuler(int, glm::vec3);
			void translateCam(int, glm::vec3);
			void rotateEulerCam(int, glm::vec3);
			void rotateCam(int, glm::quat);
			void lookAtCam(int, glm::vec3, glm::vec3);
			void setCamFov(int, GLfloat);
			void setCamNearDist(int, GLfloat);
			void setCamFarDist(int, GLfloat);

			
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