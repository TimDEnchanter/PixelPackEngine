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

	static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
		void* userData
	)
	{
		std::string type = "NONE";
		switch (messageType)
		{
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT :
			type = "GENERAL";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT :
			type = "VALIDATION";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT :
			type = "PERFORMANCE";
			break;
		}

		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			PXPK_LOG_ENGINE_ERROR("Vulkan Message ({0}): {1}", type, callbackData->pMessage);
		}
		else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			PXPK_LOG_ENGINE_WARN("Vulkan Message ({0}): {1}", type, callbackData->pMessage);
		}
		else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		{
			PXPK_LOG_ENGINE_INFO("Vulkan Message ({0}): {1}", type, callbackData->pMessage);
		}

		return VK_FALSE;
	}

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
		vkDestroyDevice(Device, nullptr);

		if (EnableValidationLayers)
		{
			auto destroyDebugMessengerFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(VulkanInstance, "vkDestroyDebugUtilsMessengerEXT");
			PXPK_ASSERT_ENGINE(destroyDebugMessengerFunc != nullptr, "Failed to find Debug messenger in Vulkan Instance!");
			destroyDebugMessengerFunc(VulkanInstance, DebugMessenger, nullptr);
		}

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
		CreateVulkanInstance();
		SelectPhysicalDevice();
		CreateLogicalDevice();
	}

	void WindowsWindow::CreateVulkanInstance()
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

		// Validation layers
		const std::vector<const char*> validationLayers =
		{
			"VK_LAYER_KHRONOS_validation"
		};

#ifdef _DEBUG
		EnableValidationLayers = true;
#else
		EnableValidationLayers = false;
#endif // DEBUG

		// Enables Vulkan debugging layer
		if (EnableValidationLayers)
		{

			uint32_t instanceLayerCount;
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
			std::vector<VkLayerProperties> instanceLayers(instanceLayerCount);
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayers.data());

			for (const char* layerName : validationLayers)
			{
				bool foundLayer = false;
				for (const auto& layerProperties : instanceLayers)
				{
					if (strcmp(layerName, layerProperties.layerName) == 0)
					{
						foundLayer = true;
						break;
					}
				}

				PXPK_ASSERT_ENGINE(foundLayer, "Validation layer not available");
			}

			createInformation.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInformation.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			createInformation.enabledLayerCount = 0;
		}

		// Set up Vulkan Extensions
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (EnableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		createInformation.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInformation.ppEnabledExtensionNames = extensions.data();

		// Setup the debug callback
		VkDebugUtilsMessengerCreateInfoEXT createMessengerInfo{};
		if (EnableValidationLayers)
		{
			createMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createMessengerInfo.pfnUserCallback = VulkanDebugCallback;
			createMessengerInfo.pUserData = &Properties;

			createInformation.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&createMessengerInfo;
		}
		else
		{
			createInformation.pNext = nullptr;
		}

		// Start a Vulkan instance
		PXPK_VERIFY_ENGINE(vkCreateInstance(&createInformation, nullptr, &VulkanInstance) == VK_SUCCESS, "Failed to create a Vulkan instance!!");

		// Connect the debug messenger to the new instance
		if (EnableValidationLayers)
		{
			auto createDebugMessengerFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(VulkanInstance, "vkCreateDebugUtilsMessengerEXT");
			PXPK_ASSERT_ENGINE(createDebugMessengerFunc != nullptr, "Failed to find Debug messenger in Vulkan Instance!");
			PXPK_VERIFY_ENGINE(createDebugMessengerFunc(VulkanInstance, &createMessengerInfo, nullptr, &DebugMessenger) == VK_SUCCESS, "Failed to connect thhe Vulkan debug messenger callback!");
		}
	}
	
	void WindowsWindow::SelectPhysicalDevice()
	{
		// Select Graphics Card to use
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(VulkanInstance, &deviceCount, nullptr);
		PXPK_ASSERT_ENGINE(deviceCount > 0, "Falied to find GPU with Vulkan support!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(VulkanInstance, &deviceCount, devices.data());

		for (const auto& device : devices)
		{
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

			// TODO: Figure out criteria for devices
			int i = 0;
			for (const auto& queueFamily : queueFamilies)
			{
				if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					PhysicalDevice = device;
					GraphicsFamilyProperties = std::make_pair(i, queueFamily);
					break;
				}
				i++;
			}

			if (PhysicalDevice != VK_NULL_HANDLE)
			{
				break;
			}
		}
		PXPK_ASSERT_ENGINE(PhysicalDevice != VK_NULL_HANDLE, "Failed to find suitable GPU!");
	}
	
	void WindowsWindow::CreateLogicalDevice()
	{
		// Assign command queues
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = GraphicsFamilyProperties.first;
		queueCreateInfo.queueCount = 1;

		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		// Assign device features
		VkPhysicalDeviceFeatures deviceFeatures{};

		// Setup the logical device
		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

		deviceCreateInfo.enabledExtensionCount = 0;

		// Create the logical device
		PXPK_VERIFY_ENGINE(vkCreateDevice(PhysicalDevice, &deviceCreateInfo, nullptr, &Device) == VK_SUCCESS, "Failed to create Vulkan logical device!");

		// Grab queues from the device
		vkGetDeviceQueue(Device, GraphicsFamilyProperties.first, 0, &GraphicsQueue);
	}
}