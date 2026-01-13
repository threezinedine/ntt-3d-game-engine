#if NTT_USE_GRAPHICS_VULKAN
#include "platforms/common.h"
#include "vulkan_common.h"

namespace ntt {
class Device;

class Semaphore
{
public:
	Semaphore(Reference<Device> pDevice);
	Semaphore(const Semaphore& other);
	Semaphore(Semaphore&& other) noexcept;
	~Semaphore();

public:
	inline VkSemaphore& GetVkSemaphore()
	{
		return m_vkSemaphore;
	}

private:
	Reference<Device> m_pDevice;
	VkSemaphore		  m_vkSemaphore;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN
