#pragma once
#include "core\WindowInterface.h"

namespace PixelPack
{
	class WindowsWindow : public WindowInterface
	{
	public:
		WindowsWindow(const WindowProperties& properties);

		virtual ~WindowsWindow();

		virtual std::string GetName() const override;
		virtual unsigned int GetWidth() const override;
		virtual unsigned int GetHeight() const override;
		virtual bool IsVSyncEnabled() const override;

		// TODO: determine the event input for window/input event
		virtual void SetEventDispatcher(std::shared_ptr<entt::dispatcher> sptr_dispatcher) override;
		virtual void SetVSync(bool enabled) override;

		virtual void OnUpdate() override;

	private:
		GLFWwindow* ptr_Window;
		WindowProperties Properties;
	};
}
