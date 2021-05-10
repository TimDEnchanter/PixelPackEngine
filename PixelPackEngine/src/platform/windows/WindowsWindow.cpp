#include "Precompile.h"
#include "WindowsWindow.h"

#include "events\WindowCloseEvent.h"
#include "events\WindowResizeEvent.h"
#include "events\input\KeyEvent.h"
#include "events\input\UnicodeInputEvent.h"
#include "events\input\MouseButtonEvent.h"
#include "events\input\MouseScrollEvent.h"
#include "events\input\MouseMoveEvent.h"
#include "shaders\ShaderManager.h"

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
		// ALWAYS WAIT
		vkDeviceWaitIdle(Handles.LogicalDevice);

		//vkDestroyDescriptorPool(Handles.LogicalDevice, Handles.DescriptorPool, nullptr);

		for (size_t i = 0; i < MaxFramesInFlight; i++)
		{
			vkDestroySemaphore(Handles.LogicalDevice, Handles.RenderCompleteSemaphores[i], nullptr);
			vkDestroySemaphore(Handles.LogicalDevice, Handles.ImageAvailableSemaphores[i], nullptr);
			vkDestroyFence(Handles.LogicalDevice, Handles.InFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(Handles.LogicalDevice, Handles.CommandPool, nullptr);

		for (auto framebuffer : Handles.SwapchainFramebuffers)
		{
			vkDestroyFramebuffer(Handles.LogicalDevice, framebuffer, nullptr);
		}

		vkDestroyPipeline(Handles.LogicalDevice, Handles.GraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(Handles.LogicalDevice, Handles.PipelineLayout, nullptr);

		vkDestroyRenderPass(Handles.LogicalDevice, Handles.RenderPass, nullptr);

		ShaderManager::GetInstance().clear();

		for (auto imageView : Handles.SwapchainImageViews)
		{
			vkDestroyImageView(Handles.LogicalDevice, imageView, nullptr);
		}

		vkDestroySwapchainKHR(Handles.LogicalDevice, Handles.Swapchain, nullptr);

		vkDestroyDevice(Handles.LogicalDevice, nullptr);

		if (Handles.EnableValidationLayers)
		{
			auto destroyDebugMessengerFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Handles.Instance, "vkDestroyDebugUtilsMessengerEXT");
			PXPK_ASSERT_ENGINE(destroyDebugMessengerFunc != nullptr, "Failed to find Debug messenger in Vulkan Instance!");
			destroyDebugMessengerFunc(Handles.Instance, Handles.DebugMessenger, nullptr);
		}

		vkDestroySurfaceKHR(Handles.Instance, Handles.Surface, nullptr);

		vkDestroyInstance(Handles.Instance, nullptr);

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

	void* WindowsWindow::GetPlatformWindow() const
	{
		return ptr_Window;
	}

	void* WindowsWindow::GetPlatformData() const
	{
		return (void*)&Handles;
	}

	void WindowsWindow::SetEventDispatcher(std::shared_ptr<entt::dispatcher> sptr_dispatcher)
	{
		Properties.sptr_Dispatcher = sptr_dispatcher;
	}

	void WindowsWindow::Update()
	{
		// Check for GLFW input events
		glfwPollEvents();

		// Send Vulkan commands to GPU
		vkWaitForFences(Handles.LogicalDevice, 1, &Handles.InFlightFences[Handles.CurrentFrame], VK_TRUE, UINT64_MAX);

		PXPK_VERIFY_ENGINE(vkAcquireNextImageKHR(Handles.LogicalDevice, Handles.Swapchain, UINT64_MAX, Handles.ImageAvailableSemaphores[Handles.CurrentFrame], VK_NULL_HANDLE, &Handles.NextImage) == VK_SUCCESS, "Failed to fetch next Vulkan image!");

		if (Handles.ImagesInFlight[Handles.NextImage] != VK_NULL_HANDLE)
		{
			vkWaitForFences(Handles.LogicalDevice, 1, &Handles.ImagesInFlight[Handles.NextImage], VK_TRUE, UINT64_MAX);
		}

		Handles.ImagesInFlight[Handles.NextImage] = Handles.InFlightFences[Handles.CurrentFrame];

		VkCommandBufferBeginInfo commandBufferBeginInfo{};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		//commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		VkRenderPassBeginInfo renderPassBeginInfo{};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = Handles.RenderPass;
		renderPassBeginInfo.framebuffer = Handles.SwapchainFramebuffers[Handles.CurrentFrame];
		renderPassBeginInfo.renderArea.offset = { 0, 0 };
		renderPassBeginInfo.renderArea.extent = Handles.Extent.value();
		renderPassBeginInfo.clearValueCount = 1;
		renderPassBeginInfo.pClearValues = &ClearColor;

		PXPK_VERIFY_ENGINE(vkResetCommandBuffer(Handles.CommandBuffers[Handles.NextImage], 0) == VK_SUCCESS, "Failed to clear Vulkan command buffer!");
		PXPK_VERIFY_ENGINE(vkBeginCommandBuffer(Handles.CommandBuffers[Handles.NextImage], &commandBufferBeginInfo) == VK_SUCCESS, "Failed to begin recording command buffer!");
	}

	void WindowsWindow::Draw()
	{
		PXPK_VERIFY_ENGINE(vkEndCommandBuffer(Handles.CommandBuffers[Handles.NextImage]) == VK_SUCCESS, "Failed to record command buffer!");

		VkSemaphore waitSemaphores[] = { Handles.ImageAvailableSemaphores[Handles.CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSemaphore signalSemaphores[] = { Handles.RenderCompleteSemaphores[Handles.CurrentFrame] };

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &Handles.CommandBuffers[Handles.NextImage];
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(Handles.LogicalDevice, 1, &Handles.InFlightFences[Handles.CurrentFrame]);

		PXPK_VERIFY_ENGINE(vkQueueSubmit(Handles.GraphicsQueue, 1, &submitInfo, Handles.InFlightFences[Handles.CurrentFrame]) == VK_SUCCESS, "Failed to submit to Vulkan command queue!");

		VkSwapchainKHR swapchains[] = { Handles.Swapchain };

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;
		presentInfo.pImageIndices = &Handles.NextImage;
		presentInfo.pResults = nullptr;

		PXPK_VERIFY_ENGINE(vkQueuePresentKHR(Handles.PresentationQueue, &presentInfo) == VK_SUCCESS, "Failed to present Vulkan image!");

		Handles.CurrentFrame = (Handles.CurrentFrame + 1) % MaxFramesInFlight;
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
		CreateSurface();
		SelectPhysicalDevice();
		CreateLogicalDevice();
		CreateSwapchain();
		CreateImageViews();
		CreateRenderPass();
		CreateDefaultPipeline();
		CreateFramebuffers();
		CreateCommandPool();
		CreateCommandBuffers();
		CreateSemaphores();
		//CreateDescriptorPool();
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

#ifdef _DEBUG
		Handles.EnableValidationLayers = true;
#else
		Handles.EnableValidationLayers = false;
#endif // DEBUG

		const std::vector<const char*> validationLayers =
		{
			"VK_LAYER_KHRONOS_validation"
		};

		// Enables Vulkan debugging layer
		if (Handles.EnableValidationLayers)
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

		if (Handles.EnableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

 		createInformation.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInformation.ppEnabledExtensionNames = extensions.data();

		// Setup the debug callback
		VkDebugUtilsMessengerCreateInfoEXT createMessengerInfo{};
		if (Handles.EnableValidationLayers)
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
		PXPK_VERIFY_ENGINE(vkCreateInstance(&createInformation, nullptr, &Handles.Instance) == VK_SUCCESS, "Failed to create a Vulkan instance!!");

		// Connect the debug messenger to the new instance
		if (Handles.EnableValidationLayers)
		{
			auto createDebugMessengerFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Handles.Instance, "vkCreateDebugUtilsMessengerEXT");
			PXPK_ASSERT_ENGINE(createDebugMessengerFunc != nullptr, "Failed to find Debug messenger in Vulkan Instance!");
			PXPK_VERIFY_ENGINE(createDebugMessengerFunc(Handles.Instance, &createMessengerInfo, nullptr, &Handles.DebugMessenger) == VK_SUCCESS, "Failed to connect thhe Vulkan debug messenger callback!");
		}
	}
	
	void WindowsWindow::SelectPhysicalDevice()
	{
		// Select Graphics Card to use
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(Handles.Instance, &deviceCount, nullptr);
		PXPK_ASSERT_ENGINE(deviceCount > 0, "Falied to find GPU with Vulkan support!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(Handles.Instance, &deviceCount, devices.data());

		// TODO: Figure out aditional criteria for devices
		for (const auto& device : devices)
		{
			// 1)Find if the device supports the neceessary queues
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

			int i = 0;
			for (const auto& queueFamily : queueFamilies)
			{
				if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					Handles.GraphicsFamilyIndex = i;
				}

				VkBool32 supportsPresentation = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, Handles.Surface, &supportsPresentation);
				if (supportsPresentation)
				{
					Handles.PresentationFamilyIndex = i;
				}

				i++;
			}

			if (!Handles.GraphicsFamilyIndex || !Handles.PresentationFamilyIndex)
			{
				continue;
			}

			// 2) Find if the device supports the necessary extensions
			uint32_t deviceExtensionCount;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &deviceExtensionCount, nullptr);
			std::vector<VkExtensionProperties> availableDeviceExtensions(deviceExtensionCount);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &deviceExtensionCount, availableDeviceExtensions.data());

			std::set<std::string> requiredDeviceExtensions(DeviceExtensions.begin(), DeviceExtensions.end());
			for (const auto& extension : availableDeviceExtensions)
			{
				requiredDeviceExtensions.erase(extension.extensionName);
			}

			if (!requiredDeviceExtensions.empty())
			{
				continue;
			}

			// 3) Find if the device supports the necessary surface formats
			uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, Handles.Surface, &formatCount, nullptr);
			std::vector<VkSurfaceFormatKHR> availableSurfaceFormats(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, Handles.Surface, &formatCount, availableSurfaceFormats.data());

			for (const auto& format : availableSurfaceFormats)
			{
				if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				{
					Handles.SurfaceFormat = format;
					break;
				}
			}

			if (!Handles.SurfaceFormat)
			{
				Handles.SurfaceFormat = availableSurfaceFormats[0];
			}

			// 4) Find if device supports the necessary presentation modes
			uint32_t modeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, Handles.Surface, &modeCount, nullptr);
			std::vector<VkPresentModeKHR> availablePresentModes(modeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, Handles.Surface, &modeCount, availablePresentModes.data());

			for (const auto& mode : availablePresentModes)
			{
				if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					Handles.PresentMode = mode;
					break;
				}
			}

			if (!Handles.PresentMode)
			{
				Handles.PresentMode = VK_PRESENT_MODE_FIFO_KHR;
			}

			// Final: assign the device if all previous conditions met
			Handles.PhysicalDevice = device;
			break;
		}
		PXPK_ASSERT_ENGINE(Handles.PhysicalDevice != VK_NULL_HANDLE, "Failed to find suitable GPU!");
	}
	
	void WindowsWindow::CreateLogicalDevice()
	{
		// Assign command queues
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> queueFamilySet = 
		{
			Handles.GraphicsFamilyIndex.value(),
			Handles.PresentationFamilyIndex.value()
		};

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : queueFamilySet)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		// Assign device features
		VkPhysicalDeviceFeatures deviceFeatures{};

		// Setup the logical device
		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

		deviceCreateInfo.enabledExtensionCount = DeviceExtensions.size();
		deviceCreateInfo.ppEnabledExtensionNames = DeviceExtensions.data();

		// Create the logical device
		PXPK_VERIFY_ENGINE(vkCreateDevice(Handles.PhysicalDevice, &deviceCreateInfo, nullptr, &Handles.LogicalDevice) == VK_SUCCESS, "Failed to create Vulkan logical device!");

		// Grab queues from the device
		vkGetDeviceQueue(Handles.LogicalDevice, Handles.GraphicsFamilyIndex.value(), 0, &Handles.GraphicsQueue);
		vkGetDeviceQueue(Handles.LogicalDevice, Handles.PresentationFamilyIndex.value(), 0, &Handles.PresentationQueue);
	}

	void WindowsWindow::CreateSurface()
	{
		PXPK_VERIFY_ENGINE(glfwCreateWindowSurface(Handles.Instance, ptr_Window, nullptr, &Handles.Surface) == VK_SUCCESS, "Failed to create a Vulkan surface!");
	}

	void WindowsWindow::CreateSwapchain()
	{
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Handles.PhysicalDevice, Handles.Surface, &surfaceCapabilities);

		if (surfaceCapabilities.currentExtent.width != UINT32_MAX)
		{
			// If not maxed out, assign current extent
			Handles.Extent = surfaceCapabilities.currentExtent;
		}
		else
		{
			// Get real extent from GLFW
			int width, height;
			glfwGetFramebufferSize(ptr_Window, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			// Enforce Vulkan's max/min bounds
			actualExtent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, actualExtent.height));

		}

		uint32_t imageCount = MaxFramesInFlight;
		if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount)
		{
			imageCount = surfaceCapabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR swapchainCreateInfo{};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = Handles.Surface;
		swapchainCreateInfo.minImageCount = imageCount;
		swapchainCreateInfo.imageFormat = Handles.SurfaceFormat.value().format;
		swapchainCreateInfo.imageColorSpace = Handles.SurfaceFormat.value().colorSpace;
		swapchainCreateInfo.imageExtent = Handles.Extent.value();
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		uint32_t queueFamilies[] = { Handles.GraphicsFamilyIndex.value(), Handles.PresentationFamilyIndex.value() };

		if (Handles.GraphicsFamilyIndex != Handles.PresentationFamilyIndex)
		{
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainCreateInfo.queueFamilyIndexCount = 2;
			swapchainCreateInfo.pQueueFamilyIndices = queueFamilies;
		}
		else
		{
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchainCreateInfo.queueFamilyIndexCount = 0;
			swapchainCreateInfo.pQueueFamilyIndices = nullptr;
		}

		swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		swapchainCreateInfo.presentMode = Handles.PresentMode.value();

		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		PXPK_VERIFY_ENGINE(vkCreateSwapchainKHR(Handles.LogicalDevice, &swapchainCreateInfo, nullptr, &Handles.Swapchain) == VK_SUCCESS, "Failed to create Vulkan swapchain!");

		vkGetSwapchainImagesKHR(Handles.LogicalDevice, Handles.Swapchain, &imageCount, nullptr);
		Handles.SwapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(Handles.LogicalDevice, Handles.Swapchain, &imageCount, Handles.SwapchainImages.data());
	}

	void WindowsWindow::CreateImageViews()
	{
		Handles.SwapchainImageViews.resize(Handles.SwapchainImages.size());

		for (size_t i = 0; i < Handles.SwapchainImages.size(); i++)
		{
			VkImageViewCreateInfo imageViewCreateInfo{};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.image = Handles.SwapchainImages[i];
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = Handles.SurfaceFormat.value().format;

			imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			PXPK_VERIFY_ENGINE(vkCreateImageView(Handles.LogicalDevice, &imageViewCreateInfo, nullptr, &Handles.SwapchainImageViews[i]) == VK_SUCCESS, "Failed to create Vulkan image view!");
		}
	}

	void WindowsWindow::CreateRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = Handles.SurfaceFormat.value().format;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference attachmentReference{};
		attachmentReference.attachment = 0;
		attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription{};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &attachmentReference;

		VkSubpassDependency subpassDependency{};
		subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		subpassDependency.dstSubpass = 0;
		subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependency.srcAccessMask = 0;
		subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDescription;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &subpassDependency;

		PXPK_VERIFY_ENGINE(vkCreateRenderPass(Handles.LogicalDevice, &renderPassInfo, nullptr, &Handles.RenderPass) == VK_SUCCESS, "Failed to create Vulkan render pass!");
	}

	void WindowsWindow::CreateDefaultPipeline()
	{
		entt::resource_handle<ShaderResource> vertHandle =  ShaderManager::GetInstance().Add(DefaultVertPath, Handles.LogicalDevice);
		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertHandle->ShaderModule;
		vertShaderStageInfo.pName = "main";

		entt::resource_handle<ShaderResource> fragHandle =  ShaderManager::GetInstance().Add(DefaultFragPath, Handles.LogicalDevice);
		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragHandle->ShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		VkPipelineVertexInputStateCreateInfo vertexInputStateInfo{};
		vertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateInfo.vertexBindingDescriptionCount = 0;
		vertexInputStateInfo.pVertexBindingDescriptions = nullptr;
		vertexInputStateInfo.vertexAttributeDescriptionCount = 0;
		vertexInputStateInfo.pVertexAttributeDescriptions = nullptr;

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo{};
		inputAssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyStateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyStateInfo.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)Handles.Extent.value().width;
		viewport.height = (float)Handles.Extent.value().height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = Handles.Extent.value();

		VkPipelineViewportStateCreateInfo viewportStateInfo{};
		viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateInfo.viewportCount = 1;
		viewportStateInfo.pViewports = &viewport;
		viewportStateInfo.scissorCount = 1;
		viewportStateInfo.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizationStateInfo{};
		rasterizationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationStateInfo.depthClampEnable = VK_FALSE;
		rasterizationStateInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizationStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizationStateInfo.lineWidth = 1.0f;
		rasterizationStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizationStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizationStateInfo.depthBiasEnable = VK_FALSE;
		rasterizationStateInfo.depthBiasConstantFactor = 0.0f;
		rasterizationStateInfo.depthBiasClamp = 0.0f;
		rasterizationStateInfo.depthBiasSlopeFactor = 0.0f;

		VkPipelineMultisampleStateCreateInfo multisampleStateInfo{};
		multisampleStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleStateInfo.sampleShadingEnable = VK_FALSE;
		multisampleStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampleStateInfo.minSampleShading = 1.0f;
		multisampleStateInfo.pSampleMask = nullptr;
		multisampleStateInfo.alphaToCoverageEnable = VK_FALSE;
		multisampleStateInfo.alphaToOneEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlendStateInfo{};
		colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendStateInfo.logicOpEnable = VK_FALSE;
		colorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
		colorBlendStateInfo.attachmentCount = 1;
		colorBlendStateInfo.pAttachments = &colorBlendAttachment;
		colorBlendStateInfo.blendConstants[0] = 0.0f;
		colorBlendStateInfo.blendConstants[1] = 0.0f;
		colorBlendStateInfo.blendConstants[2] = 0.0f;
		colorBlendStateInfo.blendConstants[3] = 0.0f;

		VkDynamicState dynamicStates[] =
		{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_LINE_WIDTH
		};

		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.dynamicStateCount = 2;
		dynamicStateInfo.pDynamicStates = dynamicStates;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		PXPK_VERIFY_ENGINE(vkCreatePipelineLayout(Handles.LogicalDevice, &pipelineLayoutInfo, nullptr, &Handles.PipelineLayout) == VK_SUCCESS, "Failed to create Vulkan pipeline layout!");

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputStateInfo;
		pipelineInfo.pInputAssemblyState = &inputAssemblyStateInfo;
		pipelineInfo.pViewportState = &viewportStateInfo;
		pipelineInfo.pRasterizationState = &rasterizationStateInfo;
		pipelineInfo.pMultisampleState = &multisampleStateInfo;
		pipelineInfo.pDepthStencilState = nullptr;
		pipelineInfo.pColorBlendState = &colorBlendStateInfo;
		pipelineInfo.pDynamicState = nullptr;
		pipelineInfo.layout = Handles.PipelineLayout;
		pipelineInfo.renderPass = Handles.RenderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		PXPK_VERIFY_ENGINE(vkCreateGraphicsPipelines(Handles.LogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &Handles.GraphicsPipeline) == VK_SUCCESS, "Failed to create Vulkan pipeline!");
	}

	void WindowsWindow::CreateFramebuffers()
	{
		Handles.SwapchainFramebuffers.resize(Handles.SwapchainImageViews.size());

		for (size_t i = 0; i < Handles.SwapchainImageViews.size(); i++)
		{
			VkImageView attachments[] = { Handles.SwapchainImageViews[i] };

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = Handles.RenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = Handles.Extent.value().width;
			framebufferInfo.height = Handles.Extent.value().height;
			framebufferInfo.layers = 1;

			PXPK_VERIFY_ENGINE(vkCreateFramebuffer(Handles.LogicalDevice, &framebufferInfo, nullptr, &Handles.SwapchainFramebuffers[i]) == VK_SUCCESS, "Failed to create Vulkan framebuffer!");
		}
	}

	void WindowsWindow::CreateCommandPool()
	{
		VkCommandPoolCreateInfo commandPoolInfo{};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.queueFamilyIndex = Handles.GraphicsFamilyIndex.value();
		commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		PXPK_VERIFY_ENGINE(vkCreateCommandPool(Handles.LogicalDevice, &commandPoolInfo, nullptr, &Handles.CommandPool) == VK_SUCCESS, "Failed to create Vulkan command pool!");
	}

	void WindowsWindow::CreateCommandBuffers()
	{
		Handles.CommandBuffers.resize(Handles.SwapchainFramebuffers.size());

		VkCommandBufferAllocateInfo commandBufferInfo{};
		commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferInfo.commandPool = Handles.CommandPool;
		commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferInfo.commandBufferCount = (uint32_t)Handles.CommandBuffers.size();

		PXPK_VERIFY_ENGINE(vkAllocateCommandBuffers(Handles.LogicalDevice, &commandBufferInfo, Handles.CommandBuffers.data()) == VK_SUCCESS, "Failed to allocate Vulkan command buffer!");

		//for (size_t i = 0; i < Handles.CommandBuffers.size(); i++)
		//{
		//	VkCommandBufferBeginInfo commandBufferBeginInfo{};
		//	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		//	//commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		//	VkRenderPassBeginInfo renderPassBeginInfo{};
		//	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		//	renderPassBeginInfo.renderPass = Handles.RenderPass;
		//	renderPassBeginInfo.framebuffer = Handles.SwapchainFramebuffers[i];
		//	renderPassBeginInfo.renderArea.offset = { 0, 0 };
		//	renderPassBeginInfo.renderArea.extent = Handles.Extent.value();
		//	renderPassBeginInfo.clearValueCount = 1;
		//	renderPassBeginInfo.pClearValues = &ClearColor;

		//	PXPK_VERIFY_ENGINE(vkBeginCommandBuffer(Handles.CommandBuffers[i], &commandBufferBeginInfo) == VK_SUCCESS, "Failed to begin recording command buffer!");
		//	vkCmdBeginRenderPass(Handles.CommandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		//	{
		//		vkCmdBindPipeline(Handles.CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, Handles.GraphicsPipeline);
		//		vkCmdDraw(Handles.CommandBuffers[i], 3, 1, 0, 0);
		//	}
		//	vkCmdEndRenderPass(Handles.CommandBuffers[i]);
		//	PXPK_VERIFY_ENGINE(vkEndCommandBuffer(Handles.CommandBuffers[i]) == VK_SUCCESS, "Failed to record command buffer!");
		//}
	}

	void WindowsWindow::CreateSemaphores()
	{
		Handles.ImageAvailableSemaphores.resize(MaxFramesInFlight);
		Handles.RenderCompleteSemaphores.resize(MaxFramesInFlight);
		Handles.InFlightFences.resize(MaxFramesInFlight);
		Handles.ImagesInFlight.resize(Handles.SwapchainImages.size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MaxFramesInFlight; i++)
		{
			PXPK_VERIFY_ENGINE(vkCreateSemaphore(Handles.LogicalDevice, &semaphoreInfo, nullptr, &Handles.RenderCompleteSemaphores[i]) == VK_SUCCESS, "Failed to create Vulkan semaphore!");
			PXPK_VERIFY_ENGINE(vkCreateSemaphore(Handles.LogicalDevice, &semaphoreInfo, nullptr, &Handles.ImageAvailableSemaphores[i]) == VK_SUCCESS, "Failed to create Vulkan semaphore!");
			PXPK_VERIFY_ENGINE(vkCreateFence(Handles.LogicalDevice, &fenceInfo, nullptr, &Handles.InFlightFences[i]) == VK_SUCCESS, "Failed to create Vulkan fence!");
		}
	}

	// TODO: Set this up to better fit my process
	void WindowsWindow::CreateDescriptorPool()
	{
		std::vector<VkDescriptorPoolSize> descriptorPoolSizes =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};
		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		descriptorPoolCreateInfo.maxSets = 1000 * descriptorPoolSizes.size();
		descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
		descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizes.data();

		PXPK_VERIFY_ENGINE(vkCreateDescriptorPool(Handles.LogicalDevice, &descriptorPoolCreateInfo, nullptr, &Handles.DescriptorPool) == VK_SUCCESS, "Failed to create Vulkan descriptor pool!");
	}
}