#include "Precompile.h"
#include "ImGuiLayer.h"

#include "core\EngineApp.h"
#include "platform\windows\WindowsWindow.h"

namespace PixelPack
{
	static void ImGuiErrorCallback(VkResult result)
	{
		if (result == VK_SUCCESS)
		{
			return;
		}
		else if (result > VK_SUCCESS)
		{
			PXPK_LOG_ENGINE_WARN("Vulkan Message (IMGUI): {1}", result);
			return;
		}
		else
		{
			PXPK_ASSERT_ENGINE(false, "Vulkan Message (IMGUI): {1}", result);
		}
	}

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

		ImGuiIO& io = ImGui::GetIO();

		// Grab app data
		EngineApp& application = EngineApp::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(application.GetWindow().get()->GetPlatformWindow());
		ptr_Handles = static_cast<VulkanHandles*>(application.GetWindow().get()->GetPlatformData());

		ImGui_ImplGlfw_InitForVulkan(window, true);

		// Create custom render pass
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = ptr_Handles->SurfaceFormat.value().format;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference attachmentReference{};
		attachmentReference.attachment = 0;
		attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription{};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &attachmentReference;

		VkSubpassDependency subpassDependency{};
		subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		subpassDependency.dstSubpass = 0;
		subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependency.srcAccessMask = 0;
		subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDescription;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &subpassDependency;

		PXPK_VERIFY_ENGINE(vkCreateRenderPass(ptr_Handles->LogicalDevice, &renderPassInfo, nullptr, &ImGuiRenderPass) == VK_SUCCESS, "Failed to create Vulkan render pass!");

		ImGuiFrameBuffers.resize(ptr_Handles->SwapchainImageViews.size());

		for (size_t i = 0; i < ptr_Handles->SwapchainImageViews.size(); i++)
		{
			VkImageView attachments[] = { ptr_Handles->SwapchainImageViews[i] };

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = ImGuiRenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = ptr_Handles->Extent.value().width;
			framebufferInfo.height = ptr_Handles->Extent.value().height;
			framebufferInfo.layers = 1;

			PXPK_VERIFY_ENGINE(vkCreateFramebuffer(ptr_Handles->LogicalDevice, &framebufferInfo, nullptr, &ImGuiFrameBuffers[i]) == VK_SUCCESS, "Failed to create Vulkan framebuffer!");
		}

		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
		pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;
		PXPK_VERIFY_ENGINE(vkCreateDescriptorPool(ptr_Handles->LogicalDevice, &pool_info, nullptr, &ImGuiDescriptorPool) == VK_SUCCESS, "Failed to create ImGui descriptor pool!");

		// Set Vulkan backend hooks
		ImGui_ImplVulkan_InitInfo initInfo = {};
		initInfo.Instance = ptr_Handles->Instance;
		initInfo.PhysicalDevice = ptr_Handles->PhysicalDevice;
		initInfo.Device = ptr_Handles->LogicalDevice;
		initInfo.QueueFamily = ptr_Handles->GraphicsFamilyIndex.value();
		initInfo.Queue = ptr_Handles->GraphicsQueue;
		initInfo.PipelineCache = VK_NULL_HANDLE;
		initInfo.DescriptorPool = ImGuiDescriptorPool;
		initInfo.MinImageCount = 2;
		initInfo.ImageCount = ptr_Handles->SwapchainImages.size();
		initInfo.CheckVkResultFn = ImGuiErrorCallback;

		ImGui_ImplVulkan_Init(&initInfo, ImGuiRenderPass);

		// Load font
		PXPK_VERIFY_ENGINE(vkResetCommandPool(ptr_Handles->LogicalDevice, ptr_Handles->CommandPool, 0) == VK_SUCCESS, "Failed to reset command pool!");

		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		PXPK_VERIFY_ENGINE(vkBeginCommandBuffer(ptr_Handles->CommandBuffers[ptr_Handles->CurrentFrame], &begin_info) == VK_SUCCESS, "Failed to begin command pool!");
		ImGui_ImplVulkan_CreateFontsTexture(ptr_Handles->CommandBuffers[ptr_Handles->CurrentFrame]);
		PXPK_VERIFY_ENGINE(vkEndCommandBuffer(ptr_Handles->CommandBuffers[ptr_Handles->CurrentFrame]) == VK_SUCCESS, "Failed to end command pool!");

		VkSubmitInfo end_info = {};
		end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		end_info.commandBufferCount = 1;
		end_info.pCommandBuffers = &ptr_Handles->CommandBuffers[ptr_Handles->CurrentFrame];
		PXPK_VERIFY_ENGINE(vkQueueSubmit(ptr_Handles->GraphicsQueue, 1, &end_info, VK_NULL_HANDLE) == VK_SUCCESS, "Failed to submit ImGui Font load caommands!");

		vkDeviceWaitIdle(ptr_Handles->LogicalDevice);

		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}

	void ImGuiLayer::Detach()
	{
		vkDeviceWaitIdle(ptr_Handles->LogicalDevice);

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		vkDestroyDescriptorPool(ptr_Handles->LogicalDevice, ImGuiDescriptorPool, nullptr);
		for (auto framebuffer : ImGuiFrameBuffers)
		{
			vkDestroyFramebuffer(ptr_Handles->LogicalDevice, framebuffer, nullptr);
		}
		vkDestroyRenderPass(ptr_Handles->LogicalDevice, ImGuiRenderPass, nullptr);
	}

	void ImGuiLayer::Update()
	{

	}

	void ImGuiLayer::Draw()
	{
		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

		// Grab app data
		PixelPack::EngineApp& application = PixelPack::EngineApp::Get();
		PixelPack::VulkanHandles* ptr_Handles = static_cast<PixelPack::VulkanHandles*>(application.GetWindow().get()->GetPlatformData());

		VkRenderPassBeginInfo renderPassBeginInfo{};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = ImGuiRenderPass;
		renderPassBeginInfo.framebuffer = ptr_Handles->SwapchainFramebuffers[ptr_Handles->CurrentFrame];
		renderPassBeginInfo.renderArea.offset = { 0, 0 };
		renderPassBeginInfo.renderArea.extent = ptr_Handles->Extent.value();
		renderPassBeginInfo.clearValueCount = 1;
		renderPassBeginInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(ptr_Handles->CommandBuffers[ptr_Handles->NextImage], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		// Start the Dear ImGui frame
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Set data for the demo window
		bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);

		// Rendering
		ImGui::Render();
		ImDrawData* draw_data = ImGui::GetDrawData();
		if (draw_data->DisplaySize.x > 0.0f && draw_data->DisplaySize.y > 0.0f)
		{
			ImGui_ImplVulkan_RenderDrawData(draw_data, ptr_Handles->CommandBuffers[ptr_Handles->CurrentFrame]);
		}

		vkCmdEndRenderPass(ptr_Handles->CommandBuffers[ptr_Handles->NextImage]);
	}

	void ImGuiLayer::SetEnabled(bool enabled)
	{
		Enabled = enabled;
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