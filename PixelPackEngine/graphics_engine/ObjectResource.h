#ifndef OBJECT_REsOURCE_H
#define OBJECT_RESOURCE_H

#include "SharedVariables.h"
#include "../utility/debugging/Logger.h"

#include <string>

namespace pxpk
{
	class ObjectResource
	{
	private:
		GLuint GLId;
		std::string sourceFilename;

	protected:
		virtual void createResource();
		virtual void deleteResource();

	public:
		ObjectResource(std::string filename);
		~ObjectResource();

		GLint getGLId();
	};
}

#endif //OBJECT_RESOURCE_H