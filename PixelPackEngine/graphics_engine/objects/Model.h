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

		//material properties
		glm::vec3 ambient = glm::vec3(0.2, 0.0, 0.1);
		glm::vec3 diffuse = glm::vec3(1.0, 0.0, 0.5);
		glm::vec3 specular = glm::vec3(1.0, 0.0, 0.5);
		float shininess = 0.75f;

		//managed resources
		std::shared_ptr<pxpk::MeshObject> meshPtr;
		std::shared_ptr<pxpk::TextureObject> texPtr;
		std::shared_ptr<pxpk::ShaderObject> shaderPtr;

	public:
		GLenum getDrawMode();
		glm::vec3 getAmbient();
		glm::vec3 getDiffuse();
		glm::vec3 getSpecular();
		float getShininess();
		std::shared_ptr<pxpk::MeshObject> getMeshPtr();
		std::shared_ptr<pxpk::TextureObject> getTexturePtr();
		std::shared_ptr<pxpk::ShaderObject> getShaderPtr();

		void setDrawMode(GLenum);
		void setAmbient(glm::vec3);
		void setDiffuse(glm::vec3);
		void setSpecular(glm::vec3);
		void setShininess(float);
		void setMeshPtr(std::shared_ptr<pxpk::MeshObject>);
		void setTexturePtr(std::shared_ptr<pxpk::TextureObject>);
		void setShaderPtr(std::shared_ptr<pxpk::ShaderObject>);

		virtual void draw();
	};
}

#endif // !MODEL_H
