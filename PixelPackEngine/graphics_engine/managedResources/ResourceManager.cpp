#include "ResourceManager.h"

pxpk::ResourceManager::ResourceManager()
{
}

pxpk::ResourceManager::~ResourceManager()
{
}

std::unordered_map<std::string, std::weak_ptr<pxpk::ObjectResource>> pxpk::ResourceManager::getMap()
{
	return observers;
}


std::shared_ptr<pxpk::MeshObject> pxpk::ResourceManager::addMesh(std::string filepath)
{
	auto it = observers.find(filepath);
	if (it != observers.end())
	{
		std::shared_ptr<pxpk::MeshObject> found = std::dynamic_pointer_cast<pxpk::MeshObject> ((*it).second.lock());
		if (found)
			return found;
	}

	std::shared_ptr<pxpk::MeshObject> newResource = std::make_shared<pxpk::MeshObject>(filepath);
	observers[filepath] = newResource;
	return newResource;
}

std::shared_ptr<pxpk::TextureObject> pxpk::ResourceManager::addTexture(std::string filepath)
{
	auto it = observers.find(filepath);
	if (it != observers.end())
	{
		std::shared_ptr<pxpk::TextureObject> found = std::dynamic_pointer_cast<pxpk::TextureObject> ((*it).second.lock());
		if (found)
			return found;
	}

	std::shared_ptr<pxpk::TextureObject> newResource = std::make_shared<pxpk::TextureObject>(filepath);
	observers[filepath] = newResource;
	return newResource;
}

std::shared_ptr<pxpk::ShaderObject> pxpk::ResourceManager::addShader(std::string filepath)
{
	auto it = observers.find(filepath);
	if (it != observers.end())
	{
		std::shared_ptr<pxpk::ShaderObject> found = std::dynamic_pointer_cast<pxpk::ShaderObject> ((*it).second.lock());
		if (found)
			return found;
	}

	std::shared_ptr<pxpk::ShaderObject> newResource = std::make_shared<pxpk::ShaderObject>(filepath);
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
