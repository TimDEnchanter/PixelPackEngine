#ifndef OBJECT_RESOURCE_H
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
		std::string filepath;

	public:
		virtual void bindResource();
		virtual void freeResource();
		virtual void deleteResource();

		ObjectResource(std::string);
		virtual ~ObjectResource();

		std::string getFilepath();
	};
}

#endif //OBJECT_RESOURCE_H