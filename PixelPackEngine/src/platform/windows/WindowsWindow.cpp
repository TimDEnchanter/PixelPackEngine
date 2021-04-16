#include "Precompile.h"
#include "WindowsWindow.h"

#include "events\WindowCloseEvent.h"
#include "events\WindowResizeEvent.h"
#include "events\input\KeyEvent.h"
#include "events\input\UnicodeInputEvent.h"
#include "events\input\MouseButtonEvent.h"
#include "events\input\MouseScrollEvent.h"
#include "events\input\MouseMoveEvent.h"

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

	static void GLFWWindowResizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);

		properties.Width = width;
		properties.Height = height;

		PXPK_ASSERT_ENGINE(properties.sptr_Dispatcher, "No dispatcher attatched to {0} window!", properties.Name);

		properties.sptr_Dispatcher->trigger<WindowResizeEvent>(width, height);
	}

	static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);

		PXPK_ASSERT_ENGINE(properties.sptr_Dispatcher, "No dispatcher attatched to {0} window!", properties.Name);

		switch (action)
		{
		case GLFW_PRESS:
			properties.sptr_Dispatcher->enqueue<KeyEvent>(key, mods, KeyAction::PRESS);
			break;
		case GLFW_REPEAT:
			properties.sptr_Dispatcher->enqueue<KeyEvent>(key, mods, KeyAction::REPEAT);
			break;
		case GLFW_RELEASE:
			properties.sptr_Dispatcher->enqueue<KeyEvent>(key, mods, KeyAction::RELEASE);
			break;
		default:
			PXPK_LOG_ENGINE_ERROR("Invalid key state ({0})", action);
			break;
		}
	}

	static void GLFWCharCallback(GLFWwindow* window, unsigned int codepoint)
	{
		WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);

		PXPK_ASSERT_ENGINE(properties.sptr_Dispatcher, "No dispatcher attatched to {0} window!", properties.Name);

		properties.sptr_Dispatcher->enqueue<UnicodeInputEvent>(codepoint);
	}

	static void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);

		PXPK_ASSERT_ENGINE(properties.sptr_Dispatcher, "No dispatcher attatched to {0} window!", properties.Name);

		switch (action)
		{
		case GLFW_PRESS:
			properties.sptr_Dispatcher->enqueue<MouseButtonEvent>(button, mods, KeyAction::PRESS);
			break;
		case GLFW_RELEASE:
			properties.sptr_Dispatcher->enqueue<MouseButtonEvent>(button, mods, KeyAction::RELEASE);
			break;
		default:
			PXPK_LOG_ENGINE_ERROR("Invalid mouse button state ({0})", action);
			break;
		}
	}

	static void GLFWScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);

		PXPK_ASSERT_ENGINE(properties.sptr_Dispatcher, "No dispatcher attatched to {0} window!", properties.Name);

		properties.sptr_Dispatcher->enqueue<MouseScrollEvent>(xOffset, yOffset);
	}

	static void GLFWCursorCallback(GLFWwindow* window, double xpos, double ypos)
	{
		WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);

		PXPK_ASSERT_ENGINE(properties.sptr_Dispatcher, "No dispatcher attatched to {0} window!", properties.Name);

		properties.sptr_Dispatcher->enqueue<MouseMoveEvent>(xpos, ypos);
	}

	// Override the interface's Create() function
	WindowInterface* WindowInterface::Create(const WindowProperties& properties)
	{
		return new WindowsWindow(properties);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
	{
		Properties = properties;

		InitWindow();
		InitVulkan();
	}

	WindowsWindow::~WindowsWindow()
	{
		vkDestroyInstance(VulkanInstance, nullptr);

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
	
	void WindowsWindow::InitWindow()
	{
		PXPK_LOG_ENGINE_INFO("Creating Window {0} ({1}, {2})", Properties.Name, Properties.Width, Properties.Height);

		if (GLFWWindowCount == 0)
		{
			PXPK_VERIFY(glfwInit(), "Failed to initialize GLFW!!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Tells GLFW that OpenGL is not being used

		ptr_Window = glfwCreateWindow((int)Properties.Width, (int)Properties.Height, Properties.Name.c_str(), nullptr, nullptr);
		GLFWWindowCount++;

		glfwSetWindowUserPointer(ptr_Window, &Properties);
		SetVSync(Properties.VSyncEnabled);

		// Setup event callbacks
		glfwSetWindowCloseCallback(ptr_Window, GLFWWindowCloseCallback);
		glfwSetWindowSizeCallback(ptr_Window, GLFWWindowResizeCallback);
		glfwSetKeyCallback(ptr_Window, GLFWKeyCallback);
		glfwSetCharCallback(ptr_Window, GLFWCharCallback);
		glfwSetMouseButtonCallback(ptr_Window, GLFWMouseButtonCallback);
		glfwSetScrollCallback(ptr_Window, GLFWScrollCallback);
		glfwSetCursorPosCallback(ptr_Window, GLFWCursorCallback);
	}
	
	void WindowsWindow::InitVulkan()
	{
		// Application Information
		VkApplicationInfo applicationInfo{};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pApplicationName = Properties.Name.c_str();
		applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pEngineName = "PixelPack Engine";
		applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.apiVersion = VK_API_VERSION_1_2;

		// Instance Information
		VkInstanceCreateInfo createInformation{};
		createInformation.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInformation.pApplicationInfo = &applicationInfo;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInformation.enabledExtensionCount = glfwExtensionCount;
		createInformation.ppEnabledExtensionNames = glfwExtensions;

		createInformation.enabledLayerCount = 0;

		// Start a Vulkan instance
		PXPK_VERIFY_ENGINE(vkCreateInstance(&createInformation, nullptr, &VulkanInstance) == VK_SUCCESS, "Failed to create a Vulkan instance!!");
	}
}