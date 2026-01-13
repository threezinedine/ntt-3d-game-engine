#if NTT_USE_GRAPHICS_VULKAN
#include "graphics/vulkan/vulkan_device.h"
#include "graphics/vulkan/vulkan_queue.h"

namespace ntt {

Device::Device(VkPhysicalDevice			 vkPhysicalDevice,
			   const Array<const char*>& extensions,
			   const Array<const char*>& layers)
	: m_vkPhysicalDevice(vkPhysicalDevice)
	, m_renderQueue(nullptr)
	, m_transferQueue(nullptr)
	, m_computeQueue(nullptr)
	, m_releaseStack()
{
	Set<u32> uniqueQueueFamilies;
	i32		 renderQueueFamilyIndex = Renderer::GetRenderQueueFamilyIndex();
	if (renderQueueFamilyIndex != -1)
	{
		uniqueQueueFamilies.emplace(renderQueueFamilyIndex);
	}

	i32 computeQueueFamilyIndex = Renderer::GetComputeQueueFamilyIndex();
	if (computeQueueFamilyIndex != -1)
	{
		uniqueQueueFamilies.emplace(computeQueueFamilyIndex);
	}

	i32 transferQueueFamilyIndex = Renderer::GetTransferQueueFamilyIndex();
	if (transferQueueFamilyIndex != -1)
	{
		uniqueQueueFamilies.emplace(transferQueueFamilyIndex);
	}

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

	VK_ASSERT(vkCreateDevice(vkPhysicalDevice, &deviceInfo, nullptr, &m_vkDevice));
	NTT_RENDERER_LOG_DEBUG("Logical device created.");
	m_releaseStack.PushReleaseFunction(NTT_NULLPTR, [&](void*) {
		vkDestroyDevice(m_vkDevice, nullptr);
		NTT_RENDERER_LOG_DEBUG("Logical device is destroyed.");
	}); // TODO: Refactor later
}

Device::~Device()
{
	m_releaseStack.ReleaseAll();
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN