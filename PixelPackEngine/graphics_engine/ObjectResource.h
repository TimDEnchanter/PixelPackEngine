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
		std::string sourceFilename;

	public:
		template <typename T> virtual void createResource(std::vector<T>);
		virtual void bindResource();
		virtual void deleteResource();

		ObjectResource(std::string);
		virtual ~ObjectResource();

		GLint getGLId();
	};
}

#endif //OBJECT_RESOURCE_H