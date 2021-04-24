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
		initInfo.DescriptorPool = handles->DescriptorPool;

		//ImGui_ImplVulkan_Init(&initInfo, )
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