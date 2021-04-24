#pragma once
#include "core\WindowInterface.h"

namespace PixelPack
{
	struct VulkanHandles
	{
		bool EnableValidationLayers = false;

		VkInstance Instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT DebugMessenger = VK_NULL_HANDLE;

		VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
		VkDevice LogicalDevice = VK_NULL_HANDLE;

		std::optional<uint32_t> GraphicsFamilyIndex;
		std::optional<uint32_t> PresentationFamilyIndex;
		VkQueue GraphicsQueue = VK_NULL_HANDLE;
		VkQueue PresentationQueue = VK_NULL_HANDLE;

		VkSurfaceKHR Surface = VK_NULL_HANDLE;
		std::optional <VkSurfaceFormatKHR> SurfaceFormat;
		std::optional <VkPresentModeKHR> PresentMode;
		std::optional <VkExtent2D> Extent;

		VkSwapchainKHR Swapchain = VK_NULL_HANDLE;
		std::vector<VkImage> SwapchainImages;
		std::vector<VkImageView> SwapchainImageViews;

		VkRenderPass RenderPass = VK_NULL_HANDLE;

		VkDescriptorPool DescriptorPool = VK_NULL_HANDLE;
	};

	class WindowsWindow : public WindowInterface
	{
	public:
		WindowsWindow(const WindowProperties& properties);

		virtual ~WindowsWindow();

		virtual std::string GetName() const override;
		virtual unsigned int GetWidth() const override;
		virtual unsigned int GetHeight() const override;

		virtual void* GetPlatformWindow() const override;
		virtual void* GetPlatformData() const override;

		// TODO: determine the event input for window/input event
		virtual void SetEventDispatcher(std::shared_ptr<entt::dispatcher> sptr_dispatcher) override;

		virtual void OnUpdate() override;

	private:
		GLFWwindow* ptr_Window;
		WindowProperties Properties;

		VulkanHandles Handles;

		const std::vector<const char*> DeviceExtensions =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

	private:
		void InitWindow();

		void InitVulkan();
		void CreateVulkanInstance();
		void SelectPhysicalDevice();
		void CreateLogicalDevice();
		void CreateSurface();
		void CreateSwapchain();
		void CreateImageViews();
		void CreateRenderPass();
		void CreateDescriptorPool();
	};
}
