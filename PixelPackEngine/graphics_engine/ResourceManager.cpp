#include "ResourceManager.h"

pxpk::ResourceManager::ResourceManager()
{
}

pxpk::ResourceManager::~ResourceManager()
{
}

std::shared_ptr<pxpk::ObjectResource> pxpk::ResourceManager::operator[](std::string filepath)
{
	auto it = observers.find(filepath);
	if (it != observers.end())
	{
		std::shared_ptr<ObjectResource> found = (*it).second.lock();
		if (found)
			return found;
	}

	std::shared_ptr<ObjectResource> newResource = std::make_shared<ObjectResource>(filepath);
	observers[filepath] = newResource;
	return newResource;
}

void pxpk::ResourceManager::flushExpired()
{
	for (auto it = observers.begin(); it != observers.end(); )
	{
		if ((*it).second.expired())
			observers.erase(it++);
		else
			it++;
	}
}
