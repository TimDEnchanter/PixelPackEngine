#pragma once

#include <string>

#include <entt\entt.hpp>

namespace PixelPack
{
	// Struct containing common window properties needed to create a basic window
	struct WindowProperties
	{
		std::string Name;
		uint32_t Width, Height;
		std::shared_ptr<entt::dispatcher> sptr_Dispatcher;

		WindowProperties
		(
			const std::string name = "PixelPack Engine",
			uint32_t width = 1920,
			uint32_t height = 1080,
			std::shared_ptr<entt::dispatcher> sptr_dispatcher = nullptr
		)
			:Name(name), Width(width), Height(height), sptr_Dispatcher(sptr_dispatcher)
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
		virtual unsigned int GetHeight() const = 0;

		virtual void* GetPlatformWindow() const = 0;
		virtual void* GetPlatformData() const = 0;

		virtual void SetEventDispatcher(std::shared_ptr<entt::dispatcher> sptr_dispatcher) = 0;

		virtual void Update() = 0;
		virtual void Draw() = 0;
	};
}
