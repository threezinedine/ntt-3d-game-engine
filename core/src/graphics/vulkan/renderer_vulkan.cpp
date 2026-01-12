#if NTT_USE_GRAPHICS_VULKAN
#include "graphics/renderer.h"
#include "graphics/surface.h"
#include "graphics/vulkan/vulkan_device.h"
#include "graphics/vulkan/vulkan_swapchain.h"

#if NTT_USE_GLFW
#include <GLFW/glfw3.h>
#endif

namespace ntt {

VkInstance		   Renderer::s_vkInstance		   = VK_NULL_HANDLE;
VkPhysicalDevice   Renderer::s_vkPhysicalDevice	   = VK_NULL_HANDLE;
Reference<Surface> Renderer::s_pSurface			   = nullptr;
Scope<Swapchain>   Renderer::s_pSwapchain		   = nullptr;
QueueFamily		   Renderer::s_renderQueueFamily   = {};
QueueFamily		   Renderer::s_computeQueueFamily  = {};
QueueFamily		   Renderer::s_transferQueueFamily = {};

#if NTT_DEBUG
VkDebugUtilsMessengerEXT Renderer::s_vkDebugMessenger = VK_NULL_HANDLE;
#endif // NTT_DEBUG

Device		 Renderer::s_device;
ReleaseStack Renderer::s_releaseStack;

void Renderer::Initialize()
{
	CreateInstance({"VK_KHR_surface"
#if NTT_DEBUG
					,
					"VK_EXT_debug_utils"
#endif // NTT_DEBUG
				   },
				   {
#if NTT_DEBUG
					   "VK_LAYER_KHRONOS_validation"
#endif // NTT_DEBUG
				   });

#if NTT_DEBUG
	void CreateDebugUtilsMessenger();
#endif // NTT_DEBUG

	ChoosePhysicalDevice();
	ChooseQueueFamilies();
}

#if NTT_DEBUG
static VkBool32 VKAPI_PTR vulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT	  messageSeverity,
											  VkDebugUtilsMessageTypeFlagsEXT			  messageTypes,
											  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
											  void*										  pUserData)
{
	NTT_UNUSED(messageTypes);
	NTT_UNUSED(pUserData);

	if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		NTT_VULKAN_LOG_ERROR("%s", pCallbackData->pMessage);
	}
	else
	{
		NTT_VULKAN_LOG_WARN("%s", pCallbackData->pMessage);
	}

	return VK_FALSE;
}

void Renderer::CreateDebugUtilsMessenger()
{
	VkDebugUtilsMessengerCreateInfoEXT messengerInfo = {};
	messengerInfo.sType								 = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	messengerInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
	messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
								VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
								VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
	messengerInfo.pfnUserCallback = vulkanDebugCallback;

	PFN_vkCreateDebugUtilsMessengerEXT createFunc =
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(s_vkInstance, "vkCreateDebugUtilsMessengerEXT");

	NTT_ASSERT(createFunc != VK_NULL_HANDLE);

	VK_ASSERT(createFunc(s_vkInstance, &messengerInfo, nullptr, &s_vkDebugMessenger));
	s_releaseStack.PushReleaseFunction(NTT_NULLPTR, [&](void*) {
		PFN_vkDestroyDebugUtilsMessengerEXT destroyFunc =
			(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(s_vkInstance, "vkDestroyDebugUtilsMessengerEXT");

		NTT_ASSERT(destroyFunc != VK_NULL_HANDLE);
		destroyFunc(s_vkInstance, s_vkDebugMessenger, nullptr);
	});
}
#endif // NTT_DEBUG

void Renderer::Shutdown()
{
	s_releaseStack.ReleaseAll();
}

void Renderer::CreateInstance(const Array<const char*>& extensions, const Array<const char*>& layers)
{
	NTT_ASSERT(s_vkInstance == VK_NULL_HANDLE);

	VkApplicationInfo appInfo  = {};
	appInfo.sType			   = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName   = "NTT Application";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName		   = "NTT Engine";
	appInfo.engineVersion	   = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion		   = VK_API_VERSION_1_0;

#if NTT_USE_GLFW
	u32			 glfwExtensionsCount = 0;
	const char** glfwExtensions		 = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

	u32				   extensionsCount = u32(extensions.size());
	Array<const char*> finalExtensions;
	finalExtensions.reserve(glfwExtensionsCount + extensionsCount);

	for (u32 glfwExtensionIndex = 0u; glfwExtensionIndex < glfwExtensionsCount; ++glfwExtensionIndex)
	{
		finalExtensions.push_back(glfwExtensions[glfwExtensionIndex]);
	}

	for (u32 extensionIndex = 0u; extensionIndex < extensionsCount; ++extensionIndex)
	{
		finalExtensions.push_back(extensions[extensionIndex]);
	}
#else
	Array<const char*> finalExtensions(extensions);
#endif

	VkInstanceCreateInfo createInfo	   = {};
	createInfo.sType				   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo		   = &appInfo;
	createInfo.enabledExtensionCount   = u32(finalExtensions.size());
	createInfo.ppEnabledExtensionNames = finalExtensions.data();
	createInfo.enabledLayerCount	   = u32(layers.size());
	createInfo.ppEnabledLayerNames	   = layers.data();

	VK_ASSERT(vkCreateInstance(&createInfo, NTT_NULLPTR, &s_vkInstance));
	s_releaseStack.PushReleaseFunction(NTT_NULLPTR, [](void*) {
		vkDestroyInstance(Renderer::GetVkInstance(), NTT_NULLPTR);
		NTT_RENDERER_LOG_DEBUG("Vulkan instance destroyed.");
	});
	NTT_RENDERER_LOG_DEBUG("Vulkan instance created.");
}

static u32 getPhysicalDeviceScore(VkPhysicalDevice physicalDevice);

void Renderer::ChoosePhysicalDevice()
{
	NTT_ASSERT(s_vkInstance != VK_NULL_HANDLE);
	NTT_ASSERT(s_vkPhysicalDevice == VK_NULL_HANDLE);

	u32 physicalDevicesCount = 0;
	VK_ASSERT(vkEnumeratePhysicalDevices(s_vkInstance, &physicalDevicesCount, nullptr));
	NTT_ASSERT(physicalDevicesCount != 0);
	Array<VkPhysicalDevice> physicalDevices(physicalDevicesCount);
	VK_ASSERT(vkEnumeratePhysicalDevices(s_vkInstance, &physicalDevicesCount, physicalDevices.data()));

	i32 bestPhysicalDeviceIndex = -1;
	u32 bestPhysicalDeviceScore = 0;

	for (u32 physicalDeviceIndex = 0u; physicalDeviceIndex < physicalDevicesCount; ++physicalDeviceIndex)
	{
		u32 score = getPhysicalDeviceScore(physicalDevices[physicalDeviceIndex]);

		if (bestPhysicalDeviceIndex < 0 || score > bestPhysicalDeviceScore)
		{
			bestPhysicalDeviceIndex = physicalDeviceIndex;
			bestPhysicalDeviceScore = score;
		}
	}

	NTT_ASSERT(bestPhysicalDeviceIndex != -1);
	NTT_ASSERT(bestPhysicalDeviceScore != 0);

	s_vkPhysicalDevice = physicalDevices[bestPhysicalDeviceIndex];
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(s_vkPhysicalDevice, &properties);

	NTT_RENDERER_LOG_DEBUG("Choose the physical %s", properties.deviceName);
}

static u32 getPhysicalDeviceScore(VkPhysicalDevice physicalDevice)
{
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(physicalDevice, &properties);

	u32 score = 0;

	if (properties.deviceType & VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		score += 1000;
	}
	else if (properties.deviceType & VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
	{
		score += 500;
	}
	else
	{
		return 0;
	}

	score += properties.limits.maxImageDimension3D;

	return score;
}

void Renderer::ChooseQueueFamilies()
{
	NTT_ASSERT(s_vkPhysicalDevice != VK_NULL_HANDLE);

	u32 queueFamiliesCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(s_vkPhysicalDevice, &queueFamiliesCount, nullptr);
	NTT_ASSERT(queueFamiliesCount != 0);

	Array<VkQueueFamilyProperties> queueFamiliesProperties(queueFamiliesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(s_vkPhysicalDevice, &queueFamiliesCount, queueFamiliesProperties.data());

	for (u32 queueFamilyIndex = 0u; queueFamilyIndex < queueFamiliesCount; ++queueFamilyIndex)
	{
		VkQueueFamilyProperties familyProperties = queueFamiliesProperties[queueFamilyIndex];

		if (!s_renderQueueFamily.exist && familyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			s_renderQueueFamily.familyIndex = queueFamilyIndex;
			s_renderQueueFamily.exist		= NTT_TRUE;
		}
	}

	NTT_ASSERT(s_renderQueueFamily.exist);

	for (u32 queueFamilyIndex = 0u; queueFamilyIndex < queueFamiliesCount; ++queueFamilyIndex)
	{
		VkQueueFamilyProperties familyProperties = queueFamiliesProperties[queueFamilyIndex];

		if (familyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			if (!s_computeQueueFamily.exist || queueFamilyIndex != s_renderQueueFamily.familyIndex)
			{
				s_computeQueueFamily.familyIndex = queueFamilyIndex;
				s_computeQueueFamily.exist		 = NTT_TRUE;
			}
		}

		if (familyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			if (!s_transferQueueFamily.exist || queueFamilyIndex != s_renderQueueFamily.familyIndex)
			{
				s_transferQueueFamily.familyIndex = queueFamilyIndex;
				s_transferQueueFamily.exist		  = NTT_TRUE;
			}
		}
	}

	NTT_RENDERER_LOG_DEBUG("Vulkan - Graphical family index: %d", s_renderQueueFamily.familyIndex);

	if (s_computeQueueFamily.exist)
	{
		NTT_RENDERER_LOG_DEBUG("Vulkan - Compute family index: %d", s_computeQueueFamily.familyIndex);
	}
	else
	{
		NTT_RENDERER_LOG_DEBUG("Vulkan - No compute support");
	}

	NTT_ASSERT(s_transferQueueFamily.exist);
	NTT_RENDERER_LOG_DEBUG("Vulkan - Transfer family index: %d", s_transferQueueFamily.familyIndex);
}

void Renderer::CreateDevice(const Array<const char*>& extensions, const Array<const char*>& layers)
{
	Set<u32> uniqueQueueFamilies;
	uniqueQueueFamilies.emplace(s_renderQueueFamily.familyIndex);
	if (s_computeQueueFamily.exist)
	{
		uniqueQueueFamilies.emplace(s_computeQueueFamily.familyIndex);
	}
	uniqueQueueFamilies.emplace(s_transferQueueFamily.familyIndex);

	Array<VkDeviceQueueCreateInfo> queueInfos;
	queueInfos.reserve(uniqueQueueFamilies.size());
	float renderQueuePriority = 1.0f;
	float otherPriority		  = 0.0f;

	for (const auto& familyIndex : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueInfo = {};
		queueInfo.sType					  = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
#if 0
		queueInfo.pQueuePriorities =
			familyIndex == s_renderQueueFamily.familyIndex ? &renderQueuePriority : &otherPriority;
#else
		NTT_UNUSED(otherPriority);
		queueInfo.pQueuePriorities = &renderQueuePriority;
#endif
		queueInfo.queueCount	   = 1;
		queueInfo.queueFamilyIndex = familyIndex;

		queueInfos.push_back(queueInfo);
	}

	VkDeviceCreateInfo deviceInfo	   = {};
	deviceInfo.sType				   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.enabledExtensionCount   = u32(extensions.size());
	deviceInfo.ppEnabledExtensionNames = extensions.data();
	deviceInfo.enabledLayerCount	   = u32(layers.size());
	deviceInfo.ppEnabledLayerNames	   = layers.data();
	deviceInfo.queueCreateInfoCount	   = u32(queueInfos.size());
	deviceInfo.pQueueCreateInfos	   = queueInfos.data();

	VK_ASSERT(vkCreateDevice(s_vkPhysicalDevice, &deviceInfo, nullptr, &s_device.GetVkDevice()));
	NTT_RENDERER_LOG_DEBUG("Logical device created.");
	s_releaseStack.PushReleaseFunction(NTT_NULLPTR, [&](void*) {
		vkDestroyDevice(s_device.GetVkDevice(), nullptr);
		NTT_RENDERER_LOG_DEBUG("Logical device is destroyed.");
	});
}

void Renderer::CheckingTheSurfaceSupport()
{
	NTT_ASSERT(s_pSurface != nullptr);
	NTT_ASSERT(s_pSurface->GetVkSurface() != VK_NULL_HANDLE);
	NTT_ASSERT(s_renderQueueFamily.exist);

	u32 support = VK_FALSE;

#if 0
	VkSurfaceCapabilitiesKHR capabilities;
	VK_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(s_vkPhysicalDevice, s_pSurface->GetVkSurface(), &capabilities));
#endif

	VK_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(
		s_vkPhysicalDevice, s_renderQueueFamily.familyIndex, s_pSurface->GetVkSurface(), &support));

	NTT_ASSERT(support == VK_TRUE);
	NTT_RENDERER_LOG_DEBUG("The surface validated.");
}

void Renderer::AttachSurface(Reference<Surface> pSurface)
{
	s_pSurface = pSurface;
	s_pSurface->CreateVkSurface();
	s_releaseStack.PushReleaseFunction(NTT_NULLPTR, [&](void*) { s_pSurface->DestroyVkSurface(); });

	if (!s_device.IsInitialized())
	{
		CreateDevice({"VK_KHR_swapchain"}, {});
		CheckingTheSurfaceSupport();
	}

	s_pSwapchain = CreateScope<Swapchain>(&s_device, s_pSurface);
	s_releaseStack.PushReleaseFunction(NTT_NULLPTR, [&](void*) {
		s_pSwapchain.reset();
		NTT_RENDERER_LOG_DEBUG("Swapchain destroyed.");
	});

	s_releaseStack.PushReleaseFunction(NTT_NULLPTR, [&](void*) {
		vkDeviceWaitIdle(s_device.GetVkDevice());
		NTT_RENDERER_LOG_DEBUG("Surface detached.");
	});
}

u32 Renderer::GetRenderQueueFamilyIndex()
{
	return s_renderQueueFamily.familyIndex;
}

void Renderer::BeginFrame()
{
}

void Renderer::EndFrame()
{
}

void Renderer::PresentFrame()
{
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN