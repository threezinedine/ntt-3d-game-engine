#if NTT_USE_GRAPHICS_OPENGL

#include "graphics/opengl/vertex_buffer_opengl.h"

namespace ntt {

VertexBuffer::VertexBuffer(VertexBufferType type)
	: m_type(type)
{
	glGenBuffers(1, &m_bufferId);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	: m_type(other.m_type)
	, m_bufferId(other.m_bufferId)
{
	other.m_type	 = VERTEX_BUFFER_TYPE_COUNT;
	other.m_bufferId = 0;
}

VertexBuffer::~VertexBuffer()
{
	if (m_bufferId != 0)
	{
		glDeleteBuffers(1, &m_bufferId);
	}
}

void VertexBuffer::Write(void* pData, u32 size)
{
	// TODO: Modify later
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferId);
	GLenum usage = (m_type == VERTEX_BUFFER_TYPE_STATIC) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, pData, usage);
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL