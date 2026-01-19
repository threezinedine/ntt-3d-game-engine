#if NTT_USE_GRAPHICS_OPENGL
#pragma once
#include "opengl_common.h"

namespace ntt {

class Surface;
class Shader;
class VertexBuffer;

class Program
{
public:
	Program(Surface* pSurface, u32 size, VertexBuffer* pBuffer);
	NTT_DELETE_COPY(Program);
	Program(Program&& other) noexcept;
	~Program();

public:
	void AttachShader(Shader&& shader);
	void Link();

	void Bind();

private:
	Surface*	  m_pSurface;
	GLuint		  m_glProgramID;
	Array<Shader> m_shaders;
	u32			  m_bufferId;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL