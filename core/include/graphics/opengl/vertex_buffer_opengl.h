#if NTT_USE_GRAPHICS_OPENGL

#include "opengl_common.h"
#include "platforms/common.h"

namespace ntt {

class VertexBuffer
{
public:
	VertexBuffer(VertexBufferType type = VertexBufferType::VERTEX_BUFFER_TYPE_STATIC);
	NTT_DELETE_COPY(VertexBuffer);
	VertexBuffer(VertexBuffer&& other) noexcept;
	~VertexBuffer();

public:
	inline u32 GetBufferId() const
	{
		return m_bufferId;
	}

public:
	void Write(void* pData, u32 size);

	void Bind();

private:
	VertexBufferType m_type;
	u32				 m_bufferId;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL