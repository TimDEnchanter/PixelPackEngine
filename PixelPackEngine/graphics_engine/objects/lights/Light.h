#ifndef LIGHT_H
#define LIGHT_H

#include "../RenderObject.h"
#include "../../managedResources/ShaderObject.h"

namespace pxpk
{
	class Light : public pxpk::RenderObject
	{
	protected:
		//shader to send data to
		std::shared_ptr<pxpk::ShaderObject> shaderPtr;

		//common across all lights
		glm::vec3 ambient = glm::vec3(1.0);
		glm::vec3 diffuse = glm::vec3(1.0);
		glm::vec3 specular = glm::vec3(1.0);

	public:
		std::shared_ptr<pxpk::ShaderObject> getShaderPtr();
		glm::vec3 getAmbient();
		glm::vec3 getDiffuse();
		glm::vec3 getSpecular();

		void setShaderPtr(std::shared_ptr<pxpk::ShaderObject>);
		void setAmbient(glm::vec3);
		void setDiffuse(glm::vec3);
		void setSpecular(glm::vec3);

		virtual void draw(int);
	};
}

#endif // !LIGHT_H
