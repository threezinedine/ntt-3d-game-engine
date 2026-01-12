#if NTT_USE_GRAPHICS_VULKAN
#include "graphics/renderer.h"
#include <vulkan/vulkan.h>

namespace ntt {

VkInstance	 Renderer::s_vkInstance = VK_NULL_HANDLE;
ReleaseStack Renderer::s_releaseStack;

void Renderer::Initialize()
{
	CreateInstance();
}

void Renderer::Shutdown()
{
	s_releaseStack.ReleaseAll();
}

void Renderer::CreateInstance()
{
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
		NTT_RENDERER_LOG_INFO("Vulkan instance destroyed.");
	});
	NTT_RENDERER_LOG_INFO("Vulkan instance created.");
}

void Renderer::AttachSurface(Reference<Surface> pSurface)
{
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