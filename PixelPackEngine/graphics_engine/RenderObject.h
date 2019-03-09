#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "SharedVariables.h"
#include "ResourceManager.h"
#include "../utility/debugging/Logger.h"
#include "../dependencies/glm/vec3.hpp"
#include "../dependencies/glm/mat4x4.hpp"
#include "../dependencies/glm/gtc/quaternion.hpp"
#include "../dependencies/glm/gtc/matrix_transform.hpp"

#include <vector>
#include <memory>

/*
RenderObject

the 3D object rendered in OpeGL.

*/

namespace pxpk {

	class RenderObject
	{
		protected:
			glm::vec3 position = glm::vec3();                      //location of origin point
			glm::quat orientation = glm::quat(0.0, 0.0, 0.0, 1.0); //orientation in quaternions
			glm::vec3 scale = glm::vec3(1.0);

		private:
			GLenum drawMode = GL_TRIANGLES;                        //draw mode of vertices

			//managed resources
			std::shared_ptr<pxpk::MeshObject> meshPtr;
			std::shared_ptr<pxpk::TextureObject> texPtr;
			std::shared_ptr<pxpk::ShaderObject> shaderPtr;

			glm::quat quatToVector(glm::vec3, glm::vec3);

		public:
			RenderObject();
			RenderObject(const RenderObject&);
			~RenderObject();

			GLenum getDrawMode();
			glm::vec3 getPosition();
			glm::quat getOrientaion();
			glm::vec3 getOrientationEuler();
			glm::vec3 getScale();
			std::shared_ptr<pxpk::MeshObject> getMeshPtr();
			std::shared_ptr<pxpk::TextureObject> getTexturePtr();
			std::shared_ptr<pxpk::ShaderObject> getShaderPtr();

			glm::mat4 getModelMatrix();

			void setDrawMode(GLenum);
			void setObjColor(glm::vec3);
			void setPosition(glm::vec3);
			void setOrientation(glm::quat);
			void setOrientationEuler(glm::vec3);
			void setScale(glm::vec3);
			void setMeshPtr(std::shared_ptr<pxpk::MeshObject>);
			void setTexturePtr(std::shared_ptr<pxpk::TextureObject>);
			void setShaderPtr(std::shared_ptr<pxpk::ShaderObject>);

			void translate(glm::vec3);
			void rotateEuler(glm::vec3);
			void rotate(glm::quat);
			virtual void lookAt(glm::vec3);

			virtual void draw();
	};
}
#endif // !RENDER_OBJECT_H