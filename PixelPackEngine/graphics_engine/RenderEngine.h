#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include "SharedVariables.h"
#include "objects/RenderObject.h"
#include "objects/Camera.h"
#include "objects/Model.h"
#include "objects/lights/Light.h"
#include "objects/lights/PointLight.h"
#include "managedResources/ResourceManager.h"
#include "../utility/debugging/Logger.h"
#include "../dependencies/glm/mat4x4.hpp"
#include "../dependencies/glm/gtc/matrix_transform.hpp"
#include "../common/queue/RenderQueue.h"
#include "../common/queue/DrawQueue.h"
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
			std::unordered_map<unsigned short, pxpk::PointLight> pointLights;

			pxpk::ResourceManager meshes;
			pxpk::ResourceManager shaders;
			pxpk::ResourceManager textures;

			unsigned short activeCam = 0;

			std::shared_ptr<ShaderObject> defaultShader;
			std::shared_ptr<TextureObject> defaultTexture;

			pxpk::DeltaTimer frameTimer;
			int frames;

			void processEvent(pxpk::QueueEvent);

		public:
			RenderEngine();
			~RenderEngine();

			unsigned short getActiveCam();

			pxpk::Model & getModel(unsigned short);
			pxpk::Camera & getCamera(unsigned short);
			pxpk::Light & getLight(unsigned short);

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