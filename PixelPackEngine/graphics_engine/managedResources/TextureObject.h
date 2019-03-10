#ifndef TEXTURE_OBJECT_H
#define TEXTURE_OBJECT_H

#include "ObjectResource.h"

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
