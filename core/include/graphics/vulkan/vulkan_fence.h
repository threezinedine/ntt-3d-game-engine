#if NTT_USE_GRAPHICS_VULKAN
#include "platforms/common.h"
#include "vulkan_common.h"

namespace ntt {
class Device;

class Fence
{
public:
	Fence(Reference<Device> pDevice);
	Fence(const Fence& other);
	Fence(Fence&& other) noexcept;
	~Fence();

public:
	inline VkFence& GetVkFence()
	{
		return m_vkFence;
	}

public:
	void Wait(u64 timeout = UINT64_MAX);
	void Reset();

private:
	Reference<Device> m_pDevice;
	VkFence			  m_vkFence;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN