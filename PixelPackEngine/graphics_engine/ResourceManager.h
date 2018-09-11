#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "ObjectResource.h"

#include <unordered_map>
#include <string>
#include <memory>

namespace pxpk
{
	class ResourceManager
	{
	private:
		std::unordered_map < std::string, std::weak_ptr<pxpk::ObjectResource> > observers;

	public:
		ResourceManager();
		~ResourceManager();

		std::shared_ptr<pxpk::ObjectResource> operator[](std::string);
		void flushExpired();
	};
}

#endif //RESOURCE_MANAGER_H