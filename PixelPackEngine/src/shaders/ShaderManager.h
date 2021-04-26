#pragma once
namespace PixelPack
{
	struct ShaderResource
	{
		VkShaderModule ShaderModule = VK_NULL_HANDLE;
		VkDevice LogicalDevice = VK_NULL_HANDLE;

		ShaderResource(std::filesystem::path filePath, VkDevice logicalDevice);
		~ShaderResource();
	};

	struct ShaderLoader : entt::resource_loader<ShaderLoader, ShaderResource>
	{
		std::shared_ptr<ShaderResource> load(std::filesystem::path filePath, VkDevice logicalDevice) const;
	};

	class ShaderManager
	{
	public:
		static ShaderManager& GetInstance();

		entt::resource_handle<ShaderResource> Add(std::filesystem::path filePath, VkDevice logicalDevice);
		entt::resource_handle<ShaderResource> Get(std::filesystem::path filePath);
		entt::resource_handle<ShaderResource> reload(std::filesystem::path filePath, VkDevice logicalDevice);
		void remove(std::filesystem::path filePath);
		bool exists(std::filesystem::path filePath);
		void clear();


	private:
		static ShaderManager* ptr_Instance;

		entt::resource_cache<ShaderResource> cache{};

	private:
		ShaderManager();
	};
}

