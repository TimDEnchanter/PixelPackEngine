#ifndef OBJECT_REsOURCE_H
#define OBJECT_RESOURCE_H

#include "SharedVariables.h"
#include "../utility/debugging/Logger.h"

#include <string>
#include <vector>

namespace pxpk
{
	class ObjectResource
	{
	protected:
		GLuint GLId;

	public:
		virtual void bindResource();
		virtual void freeResource();
		virtual void deleteResource();

		ObjectResource(std::string);
		virtual ~ObjectResource();

		GLint getGLId();
	};
}

#endif //OBJECT_RESOURCE_H