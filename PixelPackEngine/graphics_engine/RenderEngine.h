#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include "SharedVariables.h"
#include "RenderObject.h"
#include "Camera.h"
#include "Model.h"
#include "MeshObject.h"
#include "ShaderObject.h"
#include "ResourceManager.h"
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
			std::unordered_map<unsigned short, pxpk::Model> models;
			std::unordered_map<unsigned short, pxpk::Camera> cameras;

			pxpk::ResourceManager resources;

			unsigned short activeCam = 0;

			std::shared_ptr<ShaderObject> defaultShader;

			pxpk::DeltaTimer frameTimer;
			int frames;

			void processEvent(pxpk::QueueEvent);

		public:
			RenderEngine();
			~RenderEngine();

			unsigned short getActiveCam();

			pxpk::Model & getModel(unsigned short);
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