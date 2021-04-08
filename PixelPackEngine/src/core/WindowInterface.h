#pragma once

namespace PixelPack
{
	// Struct containing common window properties needed to create a basic window
	struct WindowProperties
	{
		std::string Name;
		uint32_t Width, Height;
		bool VSyncEnabled;
		entt::delegate<void(void)> Listener;

		WindowProperties
		(
			const std::string name = "PixelPack Engine",
			uint32_t width = 1920,
			uint32_t height = 1080,
			bool vsync = true,
			entt::delegate<void(void)> listener = NULL
		)
			:Name(name), Width(width), Height(height), VSyncEnabled(vsync), Listener(listener)
		{

		}
	};

	// Interface representing a desktop window
	// Implemented separately for each platform
	class WindowInterface
	{
	public:
		static WindowInterface* Create(const WindowProperties& properties = WindowProperties());

		virtual ~WindowInterface() = default;

		virtual std::string GetName() const = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int Getheight() const = 0;
		virtual bool IsVSyncEnabled() const = 0;

		// TODO: determine the event input for window/input event
		virtual void ConnectEventListener(entt::delegate<void(void)> listener) = 0;
		virtual void SetVSync(bool enabled) = 0;

		virtual void OnUpdate() = 0;
	};
}
