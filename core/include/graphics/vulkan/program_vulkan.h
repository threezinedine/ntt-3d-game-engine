#if NTT_USE_GRAPHICS_VULKAN

#include "graphics/vulkan/vulkan_common.h"
#include "platforms/common.h"

namespace ntt {

class Device;
class ReleaseStack;
class Shader;
class Surface;
class RenderPass;
class CommandBuffer;
class VertexBuffer;

class Program
{
public:
	Program(Device* pDevice, Surface* pSurface, RenderPass* pRenderPass, u32 maxImages, VertexBuffer* pBuffer);
	NTT_DELETE_COPY(Program);
	Program(Program&& other) noexcept;
	~Program();

public:
	void AttachShader(Shader&& other);

	void Link();

	void Bind(CommandBuffer& commandBuffer);

public:
	inline VkPipeline& GetVkPipeline()
	{
		return m_vkPipeline;
	}

private:
	Device*				   m_pDevice;
	Surface*			   m_pSurface;
	RenderPass*			   m_pRenderPass;
	VkPipelineLayout	   m_vkPipelineLayout;
	VkPipeline			   m_vkPipeline;
	Array<Shader>		   m_shaders;
	u32					   m_maxImages;
	VertexBuffer*		   m_pVertexBuffer;
	VkDescriptorPool	   m_vkDescriptorPool;
	VkDescriptorSetLayout  m_vkDescriptorSetLayout;
	Array<VkDescriptorSet> m_descriptorSets;

private:
	ReleaseStack m_releaseStack;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN
