#if NTT_USE_GRAPHICS_VULKAN

#include "platforms/common.h"
#include "vulkan_common.h"

namespace ntt {

class Device;
class CommandBuffer;
class Semaphore;
class Fence;
class Swapchain;

class GraphicQueue
{
public:
	GraphicQueue(Device* pDevice, VkQueue vkQueue);
	GraphicQueue(const GraphicQueue& other);
	GraphicQueue(GraphicQueue&& other) noexcept;
	~GraphicQueue();

public:
	void SubmitRender(CommandBuffer& buffer, Semaphore& waitSemaphore, Semaphore& signalSemaphore, Fence& fence);
	void SubmitPresent(Semaphore& waitSemaphore, Scope<Swapchain>& pSwapchain);

private:
	Device* m_pDevice;
	VkQueue m_vkQueue;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN