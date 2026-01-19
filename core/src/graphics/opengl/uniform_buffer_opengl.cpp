#if NTT_USE_GRAPHICS_OPENGL

#include "graphics/uniform_buffer.h"

namespace ntt {

UniformBuffer::UniformBuffer(u32 size)
{
	glGenBuffers(1, &m_glBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_glBuffer);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept
	: m_glBuffer(other.m_glBuffer)
{
	other.m_glBuffer = 0;
}

UniformBuffer::~UniformBuffer()
{
	if (m_glBuffer != 0)
	{
		glDeleteBuffers(1, &m_glBuffer);
	}
}

void UniformBuffer::Write(const void* pData, u32 size)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_glBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, pData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL