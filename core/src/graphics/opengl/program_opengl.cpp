#if NTT_USE_GRAPHICS_OPENGL

#include "graphics/opengl/program_opengl.h"
#include "graphics/shader.h"
#include "graphics/surface.h"
#include "graphics/vertex_buffer.h"

namespace ntt {

Program::Program(Surface* pSurface, u32 size, VertexBuffer* pBuffer)
	: m_pSurface(pSurface)
	, m_glProgramID(0)
	, m_bufferId(pBuffer->GetBufferId())
{
}

Program::Program(Program&& other) noexcept
	: m_pSurface(other.m_pSurface)
	, m_glProgramID(other.m_glProgramID)
	, m_shaders(std::move(other.m_shaders))
	, m_bufferId(other.m_bufferId)
{
	other.m_pSurface	= nullptr;
	other.m_glProgramID = 0;
	other.m_bufferId	= 0;
}

Program::~Program()
{
	if (m_glProgramID != 0)
	{
		glDeleteProgram(m_glProgramID);
		NTT_OPENGL_LOG_DEBUG("OpenGL Program ID %u deleted.", m_glProgramID);
	}
	m_shaders.clear();
}

void Program::AttachShader(Shader&& shader)
{
	m_shaders.push_back(std::move(shader));
}

void Program::Link()
{
	m_glProgramID = glCreateProgram();

	for (const Shader& shader : m_shaders)
	{
		GL_ASSERT(glAttachShader(m_glProgramID, shader.GetGLShaderID()));
	}

	GL_ASSERT(glLinkProgram(m_glProgramID));

	GLint isLinked = 0;
	GL_ASSERT(glGetProgramiv(m_glProgramID, GL_LINK_STATUS, &isLinked));
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		GL_ASSERT(glGetProgramiv(m_glProgramID, GL_INFO_LOG_LENGTH, &maxLength));

		Array<GLchar> infoLog(maxLength);
		GL_ASSERT(glGetProgramInfoLog(m_glProgramID, maxLength, &maxLength, &infoLog[0]));

		GL_ASSERT(glDeleteProgram(m_glProgramID));
		m_glProgramID = 0;

		NTT_OPENGL_LOG_ERROR("Failed to link program: %s", &infoLog[0]);
		NTT_UNREACHABLE();
	}

	NTT_OPENGL_LOG_DEBUG("OpenGL Program linked successfully. Program ID: %u", m_glProgramID);
}

void Program::Bind()
{
	GL_ASSERT(glUseProgram(m_glProgramID));
	GL_ASSERT(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_bufferId));
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL