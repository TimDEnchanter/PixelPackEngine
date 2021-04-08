#pragma once
#include "core\WindowInterface.h"

namespace PixelPack
{
	class VulkanWindow : public WindowInterface
	{
	public:
		VulkanWindow(const WindowProperties& properties);

		virtual ~VulkanWindow();

		virtual std::string GetName() const override;
		virtual unsigned int GetWidth() const override;
		virtual unsigned int Getheight() const override;
		virtual bool IsVSyncEnabled() const override;

		// TODO: determine the event input for window/input event
		virtual void ConnectEventListener(entt::delegate<void(void)> listener) override;
		virtual void SetVSync(bool enabled) override;

		virtual void OnUpdate() override;

	private:
		GLFWwindow* ptr_Window;
		WindowProperties Properties;
	};
}
