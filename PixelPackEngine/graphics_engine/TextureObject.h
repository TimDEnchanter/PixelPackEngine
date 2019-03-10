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

	public:
		virtual void bindResource();
		virtual void freeResource();
		virtual void deleteResource();

		TextureObject(std::string);
		virtual ~TextureObject();
	};
}

#endif // !TEXTURE_OBJECT_H
