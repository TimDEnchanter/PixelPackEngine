#include "Precompile.h"
#include "VulkanWindow.h"

namespace PixelPack
{
	static uint8_t GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		PXPK_LOG_ENGINE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	// Override the interface's Create() function
	WindowInterface* WindowInterface::Create(const WindowProperties& properties)
	{
		return new VulkanWindow(properties);
	}

	VulkanWindow::VulkanWindow(const WindowProperties& properties)
	{
		Properties = properties;

		PXPK_LOG_ENGINE_INFO("Creating Window {0} ({1}, {2})", Properties.Name, Properties.Width, Properties.Height);

		if (GLFWWindowCount == 0)
		{
			PXPK_VERIFY(glfwInit(), "Could not initialize GLFW!!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		ptr_Window = glfwCreateWindow((int)Properties.Width, (int)Properties.Height, Properties.Name.c_str(), nullptr, nullptr);
		GLFWWindowCount++;

		glfwSetWindowUserPointer(ptr_Window, &Properties);
		SetVSync(Properties.VSyncEnabled);
	}

	VulkanWindow::~VulkanWindow()
	{
		glfwDestroyWindow(ptr_Window);
		GLFWWindowCount--;

		if (GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	std::string VulkanWindow::GetName() const
	{
		return Properties.Name;
	}

	unsigned int VulkanWindow::GetWidth() const
	{
		return Properties.Width;
	}

	unsigned int VulkanWindow::Getheight() const
	{
		return Properties.Height;
	}

	bool VulkanWindow::IsVSyncEnabled() const
	{
		return Properties.VSyncEnabled;
	}

	void VulkanWindow::ConnectEventListener(entt::delegate<void(void)> listener)
	{
		// TODO: Figure this out!
	}

	void VulkanWindow::SetVSync(bool enabled)
	{
		// TODO: figure out how to do this in Vulkan.
		// Look into VK_PRESENT_MODE_FIFO_KHR
		//if (enabled)
		//{
		//	glfwSwapInterval(1);
		//}
		//else
		//{
		//	glfwSwapInterval(0);
		//}

		Properties.VSyncEnabled = enabled;
	}

	void VulkanWindow::OnUpdate()
	{
		glfwPollEvents();
	}
}