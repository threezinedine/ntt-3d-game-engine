#if NTT_USE_GRAPHICS_OPENGL

#include "graphics/opengl/shader_opengl.h"
#include "containers/release_stack.h"
#include <GL/glew.h>
#include <fstream>

namespace ntt {

Shader::Shader(const char* filename)
	: m_content()
	, m_isValid(false)
	, m_glShaderID(0)
{
	m_stage = getShaderStageFromFileName(filename);

	// check if file name is relative with NTT_ENGINE_DIRECTORY, if its absolute, use relative path only for logging
	if (std::filesystem::path(filename).is_absolute())
	{
		std::filesystem::path relativePath = std::filesystem::relative(
			std::filesystem::path(filename), std::filesystem::path(STRINGIFY(NTT_ENGINE_DIRECTORY)));
		memcpy(m_filename, relativePath.string().c_str(), SHADER_FILE_NAME_LENGTH);
	}
	else
	{
		memcpy(m_filename, filename, strlen(filename) + 1);
	}

	NTT_ASSERT(std::filesystem::exists(filename));
	std::ifstream fileStream(filename);
	NTT_ASSERT(fileStream.is_open());
	m_content = String(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());

	NTT_OPENGL_LOG_DEBUG("Shader \"%s\" is loaded", m_filename);
}

Shader::Shader(Shader&& other)
	: m_content(std::move(other.m_content))
	, m_stage(other.m_stage)
	, m_isValid(other.m_isValid)
	, m_glShaderID(other.m_glShaderID)
{
	memcpy(m_filename, other.m_filename, SHADER_FILE_NAME_LENGTH);

	other.m_stage	   = ShaderStage::SHADER_STAGE_COUNT;
	other.m_isValid	   = false;
	other.m_glShaderID = 0;
}

void Shader::Compile()
{
	NTT_OPENGL_LOG_INFO("Shader stage determined: %d", m_stage);
	switch (m_stage)
	{
	case ShaderStage::SHADER_STAGE_VERTEX:
		GL_ASSERT(m_glShaderID = glCreateShader(GL_VERTEX_SHADER));
		break;
	case ShaderStage::SHADER_STAGE_FRAGMENT:
		GL_ASSERT(m_glShaderID = glCreateShader(GL_FRAGMENT_SHADER));
		break;
	default:
		NTT_UNREACHABLE();
		return;
	}

	const char* sourceCStr = m_content.c_str();
	GL_ASSERT(glShaderSource(m_glShaderID, 1, &sourceCStr, nullptr));
	GL_ASSERT(glCompileShader(m_glShaderID));

	GLint compileStatus = GL_FALSE;
	GL_ASSERT(glGetShaderiv(m_glShaderID, GL_COMPILE_STATUS, &compileStatus));
	if (compileStatus != GL_TRUE)
	{
		GLint infoLogLength = 0;
		GL_ASSERT(glGetShaderiv(m_glShaderID, GL_INFO_LOG_LENGTH, &infoLogLength));
		String infoLog(infoLogLength, '\0');
		GL_ASSERT(glGetShaderInfoLog(m_glShaderID, infoLogLength, nullptr, infoLog.data()));
		NTT_OPENGL_LOG_ERROR("Failed to compile shader %s: %s", m_filename, infoLog.c_str());
		NTT_UNREACHABLE();
		return;
	}

	NTT_OPENGL_LOG_DEBUG("Shader %s is compiled with ID %u", m_filename, m_glShaderID);
	m_isValid = true;
}

Shader::~Shader()
{
	if (m_glShaderID != 0)
	{
		GL_ASSERT(glDeleteShader(m_glShaderID));
		NTT_OPENGL_LOG_DEBUG("Shader %s with ID %u is deleted", m_filename, m_glShaderID);
	}
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL