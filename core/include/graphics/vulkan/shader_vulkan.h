#if NTT_USE_GRAPHICS_VULKAN

#include "platforms/common.h"
#include "vulkan_common.h"

#define SHADER_FILE_NAME_LENGTH 256

namespace ntt {

class ReleaseStack;
class Device;

class Shader
{
public:
	Shader(Device* pDevice, const char* filename);
	NTT_DELETE_COPY(Shader)
	Shader(Shader&& other);
	~Shader();

public:
	void Compile();

public:
	inline VkShaderModule& GetVkShaderModule()
	{
		return m_vkShaderModule;
	}

	inline ShaderStage GetShaderStage()
	{
		return m_stage;
	}

	inline b8 IsValid()
	{
		return m_isValid;
	}

private:
	Device*		   m_pDevice;
	String		   m_content;
	ShaderStage	   m_stage;
	VkShaderModule m_vkShaderModule;
	char		   m_filename[SHADER_FILE_NAME_LENGTH];
	b8			   m_isValid;

private:
	ReleaseStack m_releaseStack;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN