#if NTT_USE_GRAPHICS_OPENGL
#pragma once

#include "opengl_common.h"

#define SHADER_FILE_NAME_LENGTH 256

namespace ntt {

class Shader
{
public:
	Shader(const char* filename);
	NTT_DELETE_COPY(Shader)
	Shader(Shader&& other);
	~Shader();

public:
	void Compile();

private:
	String		m_content;
	ShaderStage m_stage;
	char		m_filename[SHADER_FILE_NAME_LENGTH];
	b8			m_isValid;
	u32			m_glShaderID;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL