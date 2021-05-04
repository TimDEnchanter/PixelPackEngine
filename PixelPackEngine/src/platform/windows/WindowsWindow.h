#pragma once
#include "core\WindowInterface.h"

namespace PixelPack
{
	struct VulkanHandles
	{
		bool EnableValidationLayers = false;
		size_t CurrentFrame = 0;

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
		std::vector<VkFramebuffer> SwapchainFramebuffers;

		VkRenderPass RenderPass = VK_NULL_HANDLE;
		VkPipelineLayout PipelineLayout = VK_NULL_HANDLE;
		VkPipeline GraphicsPipeline = VK_NULL_HANDLE;

		VkCommandPool CommandPool = VK_NULL_HANDLE;
		std::vector<VkCommandBuffer> CommandBuffers;

		std::vector<VkSemaphore> ImageAvailableSemaphores;
		std::vector<VkSemaphore> RenderCompleteSemaphores;
		std::vector<VkFence> InFlightFences;
		std::vector<VkFence> ImagesInFlight;

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

		std::filesystem::path DefaultVertPath = "./shaders/default.vert";
		std::filesystem::path DefaultFragPath = "./shaders/default.frag";

		VkClearValue ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

		const int MaxFramesInFlight = 2;

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
		void CreateDefaultPipeline();
		void CreateFramebuffers();
		void CreateCommandPool();
		void CreateCommandBuffers();
		void CreateSemaphores();
		void CreateDescriptorPool();
	};
}
