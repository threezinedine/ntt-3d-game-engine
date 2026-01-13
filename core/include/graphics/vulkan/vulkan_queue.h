#if NTT_USE_GRAPHICS_VULKAN

#include "platforms/common.h"
#include "vulkan_common.h"

namespace ntt {

class Device;

class GraphicQueue
{
public:
	GraphicQueue(Reference<Device> pDevice, VkQueue vkQueue);
	GraphicQueue(const GraphicQueue& other);
	GraphicQueue(GraphicQueue&& other) noexcept;
	~GraphicQueue();

private:
	Reference<Device> m_pDevice;
	VkQueue			  m_vkQueue;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN