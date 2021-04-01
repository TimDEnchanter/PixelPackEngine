#ifndef SCREEN_SPACE_ELEMENT_H
#define SCREEN_SPACE_ELEMENT_H

#include "../../dependencies/glm/vec2.hpp"
#include "../managedResources/ResourceManager.h"

namespace pxpk
{
	class ScreenSpaceElement
	{
	protected:
		glm::vec2 position;
		glm::vec2 pixelSize;

		std::shared_ptr<pxpk::ShaderObject> shaderPtr;

	public:
		glm::vec2 getPosition();
		glm::vec2 getPixelSize();
		std::shared_ptr<pxpk::ShaderObject> getShaderPtr();

		void setPosition(glm::vec2);
		void setPixelSize(glm::vec2);
		void setShaderPtr(std::shared_ptr<pxpk::ShaderObject>);
	};
}

#endif // !SCREEN_SPACE_ELEMENT_H