#include "ResourceManager.h"

pxpk::ResourceManager::ResourceManager()
{
}

pxpk::ResourceManager::~ResourceManager()
{
}

std::shared_ptr<pxpk::ObjectResource> pxpk::ResourceManager::operator[](std::string filename)
{
	auto it = observers.find(filename);
	if (it != observers.end())
	{
		std::shared_ptr<ObjectResource> found = (*it).second.lock();
		if (found)
			return found;
	}

	std::shared_ptr<ObjectResource> newResource = std::make_shared<ObjectResource>(filename);
	observers[filename] = newResource;
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
