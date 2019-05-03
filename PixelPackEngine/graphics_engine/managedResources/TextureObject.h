#ifndef TEXTURE_OBJECT_H
#define TEXTURE_OBJECT_H

#include "../../dependencies/stb/stb_image.h"
#include "ObjectResource.h"

namespace pxpk
{
	class TextureObject : public ObjectResource
	{
	private:
		GLuint texId = -1;
	public:
		virtual void bindResource();
		virtual void freeResource();
		virtual void deleteResource();

		TextureObject(std::string);
		virtual ~TextureObject();

		GLuint getTexId();
	};
}

#endif // !TEXTURE_OBJECT_H
