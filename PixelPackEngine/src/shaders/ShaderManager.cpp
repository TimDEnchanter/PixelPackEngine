#include "Precompile.h"
#include "ShaderManager.h"

#include "core\EngineApp.h"
#include "platform\windows\WindowsWindow.h"

namespace PixelPack
{
	ShaderResource::ShaderResource(std::filesystem::path filePath, VkDevice logicalDevice)
	{
		PXPK_ASSERT_ENGINE(filePath.has_extension(), "Shader file path has no extension!");

		shaderc_shader_kind shaderType;
		if (filePath.extension() == ".vert")
		{
			shaderType = shaderc_shader_kind::shaderc_glsl_vertex_shader;
		}
		else if (filePath.extension() == ".frag")
		{
			shaderType = shaderc_shader_kind::shaderc_glsl_fragment_shader;
		}
		else if (filePath.extension() == ".comp")
		{
			shaderType = shaderc_shader_kind::shaderc_glsl_compute_shader;
		}
		else if (filePath.extension() == ".geom")
		{
			shaderType = shaderc_shader_kind::shaderc_glsl_geometry_shader;
		}
		else if (filePath.extension() == ".tesc")
		{
			shaderType = shaderc_shader_kind::shaderc_glsl_tess_control_shader;
		}
		else if (filePath.extension() == ".tese")
		{
			shaderType = shaderc_shader_kind::shaderc_glsl_tess_evaluation_shader;
		}
		else
		{
			PXPK_ASSERT_ENGINE(false, "Failed to determine type of shader!");
		}

		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		PXPK_ASSERT_ENGINE(file.is_open(), "Failed to open file!");

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(buffer.data(), buffer.size(), shaderType, filePath.filename().string().c_str(), options);

		PXPK_ASSERT_ENGINE(result.GetCompilationStatus() == shaderc_compilation_status_success, "Failure to compile shader code: {0}", result.GetErrorMessage());

		std::vector<uint32_t> compiledCode;
		compiledCode.assign(result.cbegin(), result.cend());

		VkShaderModuleCreateInfo moduleCreateInfo{};
		moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleCreateInfo.codeSize = compiledCode.size() * sizeof(uint32_t);
		moduleCreateInfo.pCode = compiledCode.data();

		LogicalDevice = logicalDevice;

		PXPK_VERIFY_ENGINE(vkCreateShaderModule(LogicalDevice, &moduleCreateInfo, nullptr, &ShaderModule) == VK_SUCCESS, "Failed to create Vulkan shader module!");
	}

	ShaderResource::~ShaderResource()
	{
		vkDestroyShaderModule(LogicalDevice, ShaderModule, nullptr);
	}

	std::shared_ptr<ShaderResource> ShaderLoader::load(std::filesystem::path filePath, VkDevice logicalDevice) const
	{
		
		return std::shared_ptr<ShaderResource>(new ShaderResource(filePath, logicalDevice));
	}

	ShaderManager* ShaderManager::ptr_Instance = nullptr;

	ShaderManager& ShaderManager::GetInstance()
	{
		if (!ptr_Instance)
		{
			new ShaderManager();
		}
		return *ptr_Instance;
	}

	ShaderManager::ShaderManager()
	{
		PXPK_ASSERT_ENGINE(!ptr_Instance, "ShaderManager already started!");

		ptr_Instance = this;
	}

	entt::resource_handle<ShaderResource> ShaderManager::Add(std::filesystem::path filePath, VkDevice logicalDevice)
	{
		return cache.load<ShaderLoader>(entt::hashed_string{filePath.string().c_str()}, filePath, logicalDevice);
	}

	entt::resource_handle<ShaderResource> ShaderManager::Get(std::filesystem::path filePath)
	{
		return cache.handle(entt::hashed_string{ filePath.string().c_str() });
	}

	entt::resource_handle<ShaderResource> ShaderManager::reload(std::filesystem::path filePath, VkDevice logicalDevice)
	{
		return cache.reload<ShaderLoader>(entt::hashed_string{ filePath.string().c_str() }, filePath, logicalDevice);
	}

	void ShaderManager::remove(std::filesystem::path filePath)
	{
		cache.discard(entt::hashed_string{ filePath.string().c_str() });
	}

	bool ShaderManager::exists(std::filesystem::path filePath)
	{
		return cache.contains(entt::hashed_string{ filePath.string().c_str() });
	}

	void ShaderManager::clear()
	{
		cache.clear();
	}
}