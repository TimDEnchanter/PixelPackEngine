#include "Precompile.h"
#include "TestLayer.h"

#include <spdlog\spdlog.h>

TestLayer::TestLayer(bool enabled)
{
	Enabled = enabled;
	sptr_Dispatcher = std::shared_ptr<entt::dispatcher>(new entt::dispatcher());
}

TestLayer::~TestLayer()
{
}

void TestLayer::Attatch()
{
	PXPK_LOG_INFO("TestLayer::Attatch");
	sptr_Dispatcher->sink<PixelPack::KeyEvent>().connect<&TestLayer::OnKeyEvent>(this);
}

void TestLayer::Detach()
{
	sptr_Dispatcher->sink<PixelPack::KeyEvent>().disconnect<&TestLayer::OnKeyEvent>(this);
}

void TestLayer::Update()
{
	//PXPK_LOG_INFO("TestLayer::Update");
	sptr_Dispatcher->update();
}

void TestLayer::Draw()
{
	VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Grab app data
	PixelPack::EngineApp& application = PixelPack::EngineApp::Get();
	PixelPack::VulkanHandles* ptr_Handles = static_cast<PixelPack::VulkanHandles*>(application.GetWindow().get()->GetPlatformData());

	VkRenderPassBeginInfo renderPassBeginInfo{};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = ptr_Handles->RenderPass;
	renderPassBeginInfo.framebuffer = ptr_Handles->SwapchainFramebuffers[ptr_Handles->CurrentFrame];
	renderPassBeginInfo.renderArea.offset = { 0, 0 };
	renderPassBeginInfo.renderArea.extent = ptr_Handles->Extent.value();
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(ptr_Handles->CommandBuffers[ptr_Handles->NextImage], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	// TODO: Temporary!
	vkCmdBindPipeline(ptr_Handles->CommandBuffers[ptr_Handles->NextImage], VK_PIPELINE_BIND_POINT_GRAPHICS, ptr_Handles->GraphicsPipeline);
	vkCmdDraw(ptr_Handles->CommandBuffers[ptr_Handles->NextImage], 3, 1, 0, 0);

	vkCmdEndRenderPass(ptr_Handles->CommandBuffers[ptr_Handles->NextImage]);
}

void TestLayer::SetEnabled(bool enabled)
{
	Enabled = enabled;
}

bool TestLayer::IsEnabled() const
{
	return Enabled;
}

bool TestLayer::OnEvent(entt::any event)
{
	if (event.type() == entt::type_id<PixelPack::KeyEvent>() && !sptr_Dispatcher->sink<PixelPack::KeyEvent>().empty())
	{
		PixelPack::KeyEvent& keyEvent = *(PixelPack::KeyEvent*)event.data();
		sptr_Dispatcher->enqueue<PixelPack::KeyEvent>(keyEvent);
		return true;
	}
	return false;
}

void TestLayer::OnKeyEvent(const PixelPack::KeyEvent& event)
{
	PXPK_LOG_INFO("TestLayer::OnKeyEvent {0}", event);
}