#ifndef MODEL_H
#define MODEL_H

#include "RenderObject.h"
#include "../managedResources/ResourceManager.h"

namespace pxpk
{
	class Model : public pxpk::RenderObject
	{
	private:
		//draw mode of vertices
		GLenum drawMode = GL_TRIANGLES;

		//base color
		glm::vec3 baseColor = glm::vec3(1.0, 0.0, 0.5);

		//managed resources
		std::shared_ptr<pxpk::MeshObject> meshPtr;
		std::shared_ptr<pxpk::TextureObject> texPtr;
		std::shared_ptr<pxpk::ShaderObject> shaderPtr;

	public:
		GLenum getDrawMode();
		glm::vec3 getBaseColor();
		std::shared_ptr<pxpk::MeshObject> getMeshPtr();
		std::shared_ptr<pxpk::TextureObject> getTexturePtr();
		std::shared_ptr<pxpk::ShaderObject> getShaderPtr();

		void setDrawMode(GLenum);
		void setBaseColor(glm::vec3);
		void setMeshPtr(std::shared_ptr<pxpk::MeshObject>);
		void setTexturePtr(std::shared_ptr<pxpk::TextureObject>);
		void setShaderPtr(std::shared_ptr<pxpk::ShaderObject>);

		virtual void draw();
	};
}

#endif // !MODEL_H
