#if NTT_USE_GRAPHICS_VULKAN

#pragma once
#include "vulkan_common.h"

namespace ntt {

class Device;
class Buffer;

class UniformBuffer
{
public:
	UniformBuffer(Device* pDevice, u32 size);
	NTT_DELETE_COPY(UniformBuffer)
	UniformBuffer(UniformBuffer&& other) noexcept;
	~UniformBuffer();

public:
	inline Scope<Buffer>& GetBuffer()
	{
		return m_pBuffer;
	}

private:
	Device*		  m_pDevice;
	Scope<Buffer> m_pBuffer;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN