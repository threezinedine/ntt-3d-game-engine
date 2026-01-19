#if NTT_USE_GRAPHICS_OPENGL

#pragma once
#include "opengl_common.h"
#include "platforms/common.h"

namespace ntt {

class UniformBuffer
{
public:
	UniformBuffer(u32 size);
	NTT_DELETE_COPY(UniformBuffer)
	UniformBuffer(UniformBuffer&& other) noexcept;
	~UniformBuffer();

public:
	void Write(const void* pData, u32 size);

public:
	inline GLuint GetGLBuffer() const
	{
		return m_glBuffer;
	}

private:
	GLuint m_glBuffer;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL