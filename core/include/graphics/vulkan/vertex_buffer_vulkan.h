#if NTT_USE_GRAPHICS_VULKAN
#pragma once

#include "platforms/common.h"
#include "vulkan_buffer.h"
#include "vulkan_common.h"

namespace ntt {

class Device;
class CommandBuffer;
class Buffer;

class VertexBuffer
{
public:
	VertexBuffer(Device* pDevice, u32 size, VertexBufferType type = VertexBufferType::VERTEX_BUFFER_TYPE_STATIC);
	NTT_DELETE_COPY(VertexBuffer)
	VertexBuffer(VertexBuffer&& other) noexcept;
	~VertexBuffer();

public:
	void Write(CommandBuffer& buffer, void* pData, u32 size);

	inline Scope<Buffer>& GetBuffer()
	{
		return m_pBuffer;
	}

	inline Scope<Buffer>& GetLocalBuffer()
	{
		return m_pLocalBuffer;
	}

private:
	Device*			 m_pDevice;
	VertexBufferType m_type;
	Scope<Buffer>	 m_pBuffer;
	Scope<Buffer>	 m_pLocalBuffer;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN