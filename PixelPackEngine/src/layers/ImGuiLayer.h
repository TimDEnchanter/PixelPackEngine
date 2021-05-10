#pragma once

#include "core\LayerInterface.h"

#include <imgui.h>
#include <backends\imgui_impl_glfw.h>
#include <backends\imgui_impl_vulkan.h>

#include "platform\windows\WindowsWindow.h"

namespace PixelPack
{
	class PXPK_API ImGuiLayer : public LayerInterface
	{
	public:
		ImGuiLayer(bool enabled = true);
		virtual ~ImGuiLayer();

		virtual void Attatch() override;
		virtual void Detach() override;
		virtual void Update() override;
		virtual void Draw() override;

		virtual void SetEnabled(bool enabled) override;
		virtual bool IsEnabled() const override;

		virtual bool OnEvent(entt::any event) override;

	private:
		bool Enabled;
		std::shared_ptr<entt::dispatcher> sptr_Dispatcher;

		VulkanHandles* ptr_Handles;
		VkRenderPass ImGuiRenderPass;
		std::vector<VkFramebuffer> ImGuiFrameBuffers;
		VkDescriptorPool ImGuiDescriptorPool;
	};
}
