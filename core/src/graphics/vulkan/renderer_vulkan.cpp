#if NTT_USE_GRAPHICS_VULKAN
#include "graphics/renderer.h"
#include "graphics/surface.h"
#include "graphics/vulkan/image_vulkan.h"
#include "graphics/vulkan/vulkan_command_buffer.h"
#include "graphics/vulkan/vulkan_command_pool.h"
#include "graphics/vulkan/vulkan_device.h"
#include "graphics/vulkan/vulkan_fence.h"
#include "graphics/vulkan/vulkan_framebuffer.h"
#include "graphics/vulkan/vulkan_queue.h"
#include "graphics/vulkan/vulkan_renderpass.h"
#include "graphics/vulkan/vulkan_semaphore.h"
#include "graphics/vulkan/vulkan_swapchain.h"

#if NTT_USE_GLFW
#include <GLFW/glfw3.h>
#endif

namespace ntt {

VkInstance			 Renderer::s_vkInstance			 = VK_NULL_HANDLE;
VkPhysicalDevice	 Renderer::s_vkPhysicalDevice	 = VK_NULL_HANDLE;
Reference<Surface>	 Renderer::s_pSurface			 = nullptr;
Scope<Swapchain>	 Renderer::s_pSwapchain			 = nullptr;
QueueFamily			 Renderer::s_renderQueueFamily	 = {};
QueueFamily			 Renderer::s_computeQueueFamily	 = {};
QueueFamily			 Renderer::s_transferQueueFamily = {};
Scope<RenderPass>	 Renderer::s_pRenderPass		 = nullptr;
Array<Fence>		 Renderer::s_fences;
Array<Semaphore>	 Renderer::s_imageReadySemaphores;
Array<Semaphore>	 Renderer::s_renderFinisedSemaphores;
Array<CommandBuffer> Renderer::s_renderCommandBuffers;
Array<CommandBuffer> Renderer::s_presentCommandBuffers;
Array<Framebuffer>	 Renderer::s_framebuffers;

u32 Renderer::s_currentFlight = 0;

#if NTT_DEBUG
VkDebugUtilsMessengerEXT Renderer::s_vkDebugMessenger = VK_NULL_HANDLE;
#endif // NTT_DEBUG

Reference<Device> Renderer::s_pDevice = nullptr;

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

	if (s_pDevice == nullptr)
	{
		Array<const char*> extensions = {"VK_KHR_swapchain"};
		Array<const char*> layers	  = {};
		s_pDevice					  = CreateRef<Device>(s_vkPhysicalDevice, pSurface, extensions, layers);
		CheckingTheSurfaceSupport();
		s_releaseStack.PushReleaseFunction(NTT_NULLPTR, [&](void*) {
			s_pDevice.reset();
			NTT_RENDERER_LOG_DEBUG("Logical device destroyed.");
		});
	}

	s_pSwapchain = CreateScope<Swapchain>(s_pDevice, s_pSurface);
	s_releaseStack.PushReleaseFunction(NTT_NULLPTR, [&](void*) {
		s_pSwapchain.reset();
		NTT_RENDERER_LOG_DEBUG("Swapchain destroyed.");
	});

	CreateSyncObjects();

	s_renderCommandBuffers = s_pDevice->GetRenderCommandPool()->AllocateCommandBuffers(s_pSwapchain->GetImageCounts());

	s_releaseStack.PushReleaseFunction(nullptr, [&](void*) {
		s_pDevice->GetRenderCommandPool()->FreeCommandBuffers(s_pSwapchain->GetImageCounts(),
															  s_renderCommandBuffers.data());
	});

	s_presentCommandBuffers =
		s_pDevice->GetPresentCommandPool()->AllocateCommandBuffers(s_pSwapchain->GetImageCounts());

	s_releaseStack.PushReleaseFunction(nullptr, [&](void*) {
		s_pDevice->GetPresentCommandPool()->FreeCommandBuffers(s_pSwapchain->GetImageCounts(),
															   s_presentCommandBuffers.data());
	});

	s_pRenderPass = CreateScope<RenderPass>(s_pDevice, s_pSurface);
	s_releaseStack.PushReleaseFunction(nullptr, [&](void*) { s_pRenderPass.reset(); });

	CreateFrameBuffers();

	s_releaseStack.PushReleaseFunction(NTT_NULLPTR, [&](void*) {
		vkDeviceWaitIdle(s_pDevice->GetVkDevice());
		NTT_RENDERER_LOG_DEBUG("Surface detached.");
	});
}

void Renderer::CreateSyncObjects()
{
	u32 swapchainImagesCount = s_pSwapchain->GetImageCounts();

	s_fences.reserve(swapchainImagesCount);
	s_imageReadySemaphores.reserve(swapchainImagesCount);
	s_renderFinisedSemaphores.reserve(swapchainImagesCount);

	for (u32 imageIndex = 0u; imageIndex < swapchainImagesCount; ++imageIndex)
	{
		s_fences.emplace_back(s_pDevice);
		s_imageReadySemaphores.emplace_back(s_pDevice);
		s_renderFinisedSemaphores.emplace_back(s_pDevice);
	}

	s_releaseStack.PushReleaseFunction(NTT_NULLPTR, [&](void*) {
		s_fences.clear();
		s_imageReadySemaphores.clear();
		s_renderFinisedSemaphores.clear();
	});
}

void Renderer::CreateFrameBuffers()
{
	s_framebuffers.reserve(s_pSwapchain->GetImageCounts());

	for (u32 imageIndex = 0u; imageIndex < s_pSwapchain->GetImageCounts(); ++imageIndex)
	{
		s_framebuffers.emplace_back(
			s_pDevice, s_pRenderPass.get(), s_pSurface.get(), &s_pSwapchain->GetImageByIndex(imageIndex));
	}

	s_releaseStack.PushReleaseFunction(nullptr, [&](void*) { s_framebuffers.clear(); });
}

i32 Renderer::GetRenderQueueFamilyIndex()
{
	return s_renderQueueFamily.exist ? s_renderQueueFamily.familyIndex : -1;
}

i32 Renderer::GetComputeQueueFamilyIndex()
{
	return s_computeQueueFamily.exist ? s_computeQueueFamily.familyIndex : -1;
}

i32 Renderer::GetTransferQueueFamilyIndex()
{
	return s_transferQueueFamily.exist ? s_transferQueueFamily.familyIndex : -1;
}

void Renderer::BeginFrame()
{
	Fence& fence = s_fences[s_currentFlight];
	fence.Wait();
	fence.Reset();

	s_pSwapchain->AcquireNextImage(s_imageReadySemaphores[s_currentFlight]);

	CommandBuffer& buffer = s_renderCommandBuffers[s_currentFlight];
	buffer.StartRecord();

	s_pRenderPass->Begin(buffer, s_framebuffers[s_pSwapchain->GetCurrentImageIndex()], {.1f, 0.1f, 0.0f, 1.0f});
}

void Renderer::EndFrame()
{
	CommandBuffer& buffer = s_renderCommandBuffers[s_currentFlight];

	s_pRenderPass->End(buffer);
	buffer.EndRecord();
	s_pDevice->GetRenderQueue()->SubmitRender(buffer,
											  s_imageReadySemaphores[s_currentFlight],
											  s_renderFinisedSemaphores[s_currentFlight],
											  s_fences[s_currentFlight]);
}

void Renderer::PresentFrame()
{
	s_pDevice->GetPresentQueue()->SubmitPresent(s_renderFinisedSemaphores[s_currentFlight], s_pSwapchain);
	s_currentFlight = (s_currentFlight + 1) % s_pSwapchain->GetImageCounts();
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN