#if NTT_USE_GRAPHICS_VULKAN
#include "graphics/renderer.h"
#include "graphics/vulkan/vulkan_device.h"

namespace ntt {

VkInstance		   Renderer::s_vkInstance		   = VK_NULL_HANDLE;
VkPhysicalDevice   Renderer::s_vkPhysicalDevice	   = VK_NULL_HANDLE;
Reference<Surface> Renderer::s_pSurface			   = nullptr;
QueueFamily		   Renderer::s_renderQueueFamily   = {};
QueueFamily		   Renderer::s_computeQueueFamily  = {};
QueueFamily		   Renderer::s_transferQueueFamily = {};

Device		 Renderer::s_device;
ReleaseStack Renderer::s_releaseStack;

void Renderer::Initialize()
{
	CreateInstance();
	ChoosePhysicalDevice();
	ChooseQueueFamilies();
}

void Renderer::Shutdown()
{
	s_releaseStack.ReleaseAll();
}

void Renderer::CreateInstance()
{
	NTT_ASSERT(s_vkInstance == VK_NULL_HANDLE);

	VkApplicationInfo appInfo  = {};
	appInfo.sType			   = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName   = "NTT Application";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName		   = "NTT Engine";
	appInfo.engineVersion	   = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion		   = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType				= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo		= &appInfo;

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

void Renderer::CreateDevice()
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
		queueInfo.pQueuePriorities =
			familyIndex == s_renderQueueFamily.familyIndex ? &renderQueuePriority : &otherPriority;
		queueInfo.queueCount	   = 1;
		queueInfo.queueFamilyIndex = familyIndex;

		queueInfos.push_back(queueInfo);
	}

	VkDeviceCreateInfo deviceInfo	   = {};
	deviceInfo.sType				   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.enabledExtensionCount   = 0;
	deviceInfo.ppEnabledExtensionNames = nullptr;
	deviceInfo.enabledLayerCount	   = 0;
	deviceInfo.ppEnabledLayerNames	   = nullptr;
	deviceInfo.queueCreateInfoCount	   = u32(queueInfos.size());
	deviceInfo.pQueueCreateInfos	   = queueInfos.data();

	VK_ASSERT(vkCreateDevice(s_vkPhysicalDevice, &deviceInfo, nullptr, &s_device.GetVkDevice()));
	NTT_RENDERER_LOG_DEBUG("Logical device created.");
	s_releaseStack.PushReleaseFunction(NTT_NULLPTR, [&](void*) {
		vkDestroyDevice(s_device.GetVkDevice(), nullptr);
		NTT_RENDERER_LOG_DEBUG("Logical device is destroyed.");
	});
}

void Renderer::AttachSurface(Reference<Surface> pSurface)
{
	s_pSurface = pSurface;

	if (!s_device.IsInitialized())
	{
		CreateDevice();
	}
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