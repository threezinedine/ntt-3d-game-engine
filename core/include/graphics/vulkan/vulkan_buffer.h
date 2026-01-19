#if NTT_USE_GRAPHICS_VULKAN

#include "graphics/vulkan/vulkan_common.h"
#include "platforms/common.h"

namespace ntt {

class Buffer
{
public:
	Buffer(Device* pDevice, u32 size, VkBufferUsageFlags usage, VkMemoryPropertyFlags flags);
	NTT_DELETE_COPY(Buffer)
	Buffer(Buffer&& other);
	~Buffer();

public:
	inline VkBuffer& GetVkBuffer()
	{
		return m_vkBuffer;
	}

public:
	void Write(void* pData, u32 size);

private:
	i32 GetMemoryTypeIndex(u32 typeBits, VkMemoryPropertyFlags flags);

private:
	Device*		   m_pDevice;
	VkBuffer	   m_vkBuffer;
	VkDeviceMemory m_vkMemory;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN