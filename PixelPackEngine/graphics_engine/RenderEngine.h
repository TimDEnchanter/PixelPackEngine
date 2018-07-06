#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include "SharedVariables.h"
#include "RenderObject.h"
#include "Camera.h"
#include "../utility/debugging/Logger.h"
#include "../dependencies/glm/mat4x4.hpp"
#include "../dependencies/glm/gtc/matrix_transform.hpp"
#include "../common/RenderQueue.h"
#include "../common/DrawQueue.h"
#include "../common/DeltaTimer.h"
#include "../common/Inputs.h"

#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <condition_variable>
#include <iterator>

/*
RenderEngine

handles main rendernig operation
*/
namespace pxpk {

	extern bool engineStarted;

	class RenderEngine
	{
		private:
			std::unordered_map<unsigned short, pxpk::RenderObject> objects;
			std::unordered_map<unsigned short, pxpk::Camera> cameras;

			unsigned short activeCam = 0;

			GLuint programID;
			GLuint vertShaderID;
			GLuint fragShaderID;

			pxpk::DeltaTimer frameTimer;
			int frames;

			void addObject(unsigned short);
			void addCamera(unsigned short);
			void removeObject(unsigned short);
			void removeCamera(unsigned short);
			void clearObjects();
			void clearCameras();

			void setObjVertexBuffer(unsigned short, std::vector<GLfloat>);
			void setObjElementBuffer(unsigned short, std::vector<GLuint>);
			void setObjColorBuffer(unsigned short, std::vector<GLfloat>);
			void setObjColor(unsigned short, glm::vec3);
			void setObjPosition(unsigned short, glm::vec3);
			void setObjOrientation(unsigned short, glm::quat);
			void setObjOrientationEuler(unsigned short, glm::vec3);
			void setObjScale(unsigned short, glm::vec3);
			void translateObj(unsigned short, glm::vec3);
			void rotateEulerObj(unsigned short, glm::vec3);
			void rotateObj(unsigned short, glm::quat);
			void lookAtObj(unsigned short, glm::vec3);
			void drawObj(unsigned short);

			void setCamPosition(unsigned short, glm::vec3);
			void setCamOrientation(unsigned short, glm::quat);
			void setCamOrientationEuler(unsigned short, glm::vec3);
			void translateCam(unsigned short, glm::vec3);
			void rotateEulerCam(unsigned short, glm::vec3);
			void rotateCam(unsigned short, glm::quat);
			void lookAtCam(unsigned short, glm::vec3, glm::vec3);
			void setCamFov(unsigned short, GLfloat);
			void setCamNearDist(unsigned short, GLfloat);
			void setCamFarDist(unsigned short, GLfloat);

			void setActiveCam(GLuint);

			void loadShaders();
			void processEvent(pxpk::QueueEvent);

			void checkGLError(const char *file, int line);

		public:
			RenderEngine();
			~RenderEngine();

			unsigned short getActiveCam();

			pxpk::RenderObject & getObject(unsigned short);
			pxpk::Camera & getCamera(unsigned short);

			void init(int argc, char **argv, std::string windowName);
			void startEngine(int argc, char **argv, std::string windowName);

			void render();
			static void renderCallback();
			static void idleCallback();
	};


	namespace {
		//constant instance pointer
		RenderEngine *renderEngineInstance = NULL;
	}
}


#endif // !RENDER_ENGINE_H