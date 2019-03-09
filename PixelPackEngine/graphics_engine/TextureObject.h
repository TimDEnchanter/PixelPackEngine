#ifndef TEXTURE_OBJECT_H
#define TEXTURE_OBJECT_H

#include "ObjectResource.h"
#include "../dependencies/glm/vec3.hpp"
#include "../dependencies/glm/gtc/type_ptr.hpp"

namespace pxpk
{
	class TextureObject : public ObjectResource
	{
	private:
		glm::vec3 baseColor;

	public:
		virtual void bindResource();
		virtual void freeResource();
		virtual void deleteResource();

		TextureObject(std::string);
		virtual ~TextureObject();

		glm::vec3 getBaseColor();

		void setBaseColor(glm::vec3);
	};
}

#endif // !TEXTURE_OBJECT_H
