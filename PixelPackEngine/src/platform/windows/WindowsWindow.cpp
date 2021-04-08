#include "Precompile.h"
#include "WindowsWindow.h"

#include "events\WindowCloseEvent.h"

namespace PixelPack
{
	static uint8_t GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		PXPK_LOG_ENGINE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	static void GLFWWindowCloseCallback(GLFWwindow* window)
	{
		WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);

		PXPK_ASSERT_ENGINE(properties.sptr_Dispatcher, "No dispatcher attatched to {0} window!", properties.Name);

		properties.sptr_Dispatcher->trigger<WindowCloseEvent>();
	}

	// Override the interface's Create() function
	WindowInterface* WindowInterface::Create(const WindowProperties& properties)
	{
		return new WindowsWindow(properties);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
	{
		Properties = properties;

		PXPK_LOG_ENGINE_INFO("Creating Window {0} ({1}, {2})", Properties.Name, Properties.Width, Properties.Height);

		if (GLFWWindowCount == 0)
		{
			PXPK_VERIFY(glfwInit(), "Could not initialize GLFW!!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Tells GLFW that OpenGL is not being used

		ptr_Window = glfwCreateWindow((int)Properties.Width, (int)Properties.Height, Properties.Name.c_str(), nullptr, nullptr);
		GLFWWindowCount++;

		glfwSetWindowUserPointer(ptr_Window, &Properties);
		SetVSync(Properties.VSyncEnabled);

		glfwSetWindowCloseCallback(ptr_Window, GLFWWindowCloseCallback);
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(ptr_Window);
		GLFWWindowCount--;

		if (GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	std::string WindowsWindow::GetName() const
	{
		return Properties.Name;
	}

	unsigned int WindowsWindow::GetWidth() const
	{
		return Properties.Width;
	}

	unsigned int WindowsWindow::GetHeight() const
	{
		return Properties.Height;
	}

	bool WindowsWindow::IsVSyncEnabled() const
	{
		return Properties.VSyncEnabled;
	}

	void WindowsWindow::SetEventDispatcher(std::shared_ptr<entt::dispatcher> sptr_dispatcher)
	{
		Properties.sptr_Dispatcher = sptr_dispatcher;
	}

	// TODO: figure out how to do this in Vulkan.
	void WindowsWindow::SetVSync(bool enabled)
	{
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

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
	}
}