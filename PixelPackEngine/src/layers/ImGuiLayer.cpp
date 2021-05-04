#include "Precompile.h"
#include "ImGuiLayer.h"

#include "core\EngineApp.h"
#include "platform\windows\WindowsWindow.h"

namespace PixelPack
{
	ImGuiLayer::ImGuiLayer(bool enabled)
	{
		Enabled = enabled;
	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::Attatch()
	{
		// Start ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Set configs
		ImGui::StyleColorsDark();

		// Grab app data
		EngineApp& application = EngineApp::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(application.GetWindow().get()->GetPlatformWindow());
		VulkanHandles* handles = static_cast<VulkanHandles*>(application.GetWindow().get()->GetPlatformData());

		ImGui_ImplGlfw_InitForVulkan(window, true);

		// Set Vulkan backend hooks
		ImGui_ImplVulkan_InitInfo initInfo = {};
		initInfo.Instance = handles->Instance;
		initInfo.PhysicalDevice = handles->PhysicalDevice;
		initInfo.Device = handles->LogicalDevice;
		initInfo.QueueFamily = handles->GraphicsFamilyIndex.value();
		initInfo.Queue = handles->GraphicsQueue;
		//initInfo.PipelineCache = VK_NULL_HANDLE;
		//initInfo.DescriptorPool = VK_NULL_HANDLE;
		initInfo.MinImageCount = 2;
		initInfo.ImageCount = handles->SwapchainImages.size();

		ImGui_ImplVulkan_Init(&initInfo, handles->RenderPass);

		PXPK_VERIFY_ENGINE(vkResetCommandPool(handles->LogicalDevice, handles->CommandPool, 0) == VK_SUCCESS, "Failed to reset command pool!");

		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		//PXPK_VERIFY_ENGINE(vkBeginCommandBuffer(handles->CommandBuffers))
	}

	void ImGuiLayer::Detach()
	{

	}

	void ImGuiLayer::Update()
	{

	}

	void ImGuiLayer::SetEnabled(bool enabled)
	{

	}

	bool ImGuiLayer::IsEnabled() const
	{
		return Enabled;
	}

	bool ImGuiLayer::OnEvent(entt::any event)
	{
		return false;
	}
}