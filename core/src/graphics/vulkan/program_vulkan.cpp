#if NTT_USE_GRAPHICS_VULKAN

#include "graphics/vulkan/program_vulkan.h"
#include "containers/release_stack.h"
#include "graphics/types.h"
#include "graphics/vertex_buffer.h"
#include "graphics/vulkan/shader_vulkan.h"
#include "graphics/vulkan/surface_vulkan.h"
#include "graphics/vulkan/vulkan_command_buffer.h"
#include "graphics/vulkan/vulkan_device.h"
#include "graphics/vulkan/vulkan_renderpass.h"

namespace ntt {

static VkShaderStageFlagBits getShaderStageBitFromStageType(ShaderStage type);

Program::Program(Device* pDevice, Surface* pSurface, RenderPass* pRenderPass, u32 maxImages, VertexBuffer* pBuffer)
	: m_pDevice(pDevice)
	, m_pSurface(pSurface)
	, m_pRenderPass(pRenderPass)
	, m_vkPipelineLayout(VK_NULL_HANDLE)
	, m_vkPipeline(VK_NULL_HANDLE)
	, m_maxImages(maxImages)
	, m_pVertexBuffer(nullptr)
	, m_vkDescriptorPool(VK_NULL_HANDLE)
	, m_vkDescriptorSetLayout(VK_NULL_HANDLE)
{
#if 1
	VkDescriptorPoolSize poolSize = {};
	poolSize.descriptorCount	  = maxImages;
	poolSize.type				  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
#endif

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType						= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags						= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.maxSets					= maxImages;
#if 1
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes	   = &poolSize;
#else
	poolInfo.poolSizeCount = 0;
	poolInfo.pPoolSizes	   = nullptr;
#endif

	VK_ASSERT(vkCreateDescriptorPool(m_pDevice->GetVkDevice(), &poolInfo, nullptr, &m_vkDescriptorPool));
	m_releaseStack.PushReleaseFunction(
		nullptr, [&](void*) { vkDestroyDescriptorPool(m_pDevice->GetVkDevice(), m_vkDescriptorPool, nullptr); });

	VkDescriptorSetLayoutBinding binding = {};
	binding.binding						 = 0;
	binding.descriptorCount				 = 1;
	binding.descriptorType				 = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	binding.stageFlags					 = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
	setLayoutInfo.sType							  = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setLayoutInfo.bindingCount					  = 1;
	setLayoutInfo.pBindings						  = &binding;

	VK_ASSERT(vkCreateDescriptorSetLayout(m_pDevice->GetVkDevice(), &setLayoutInfo, nullptr, &m_vkDescriptorSetLayout));
	m_releaseStack.PushReleaseFunction(nullptr, [&](void*) {
		vkDestroyDescriptorSetLayout(m_pDevice->GetVkDevice(), m_vkDescriptorSetLayout, nullptr);
	});

	m_descriptorSets.resize(maxImages);
	Array<VkDescriptorSetLayout> layouts(maxImages, m_vkDescriptorSetLayout);
	VkDescriptorSetAllocateInfo	 allocateInfo = {};
	allocateInfo.sType						  = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocateInfo.descriptorPool				  = m_vkDescriptorPool;
	allocateInfo.descriptorSetCount			  = maxImages;
	allocateInfo.pSetLayouts				  = layouts.data();

	VK_ASSERT(vkAllocateDescriptorSets(m_pDevice->GetVkDevice(), &allocateInfo, m_descriptorSets.data()));

	// Allocate sets
	for (u32 imageIndex = 0u; imageIndex < maxImages; ++imageIndex)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer				  = pBuffer->GetLocalBuffer()->GetVkBuffer();
		bufferInfo.offset				  = 0;
		bufferInfo.range				  = VK_WHOLE_SIZE;

		VkWriteDescriptorSet write = {};
		write.sType				   = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorCount	   = 1;
		write.descriptorType	   = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		write.dstArrayElement	   = 0;
		write.dstSet			   = m_descriptorSets[imageIndex];
		write.dstBinding		   = 0;
		write.pBufferInfo		   = &bufferInfo;

		vkUpdateDescriptorSets(m_pDevice->GetVkDevice(), 1, &write, 0, nullptr);
	}

	m_releaseStack.PushReleaseFunction(nullptr, [&](void*) {
		vkFreeDescriptorSets(m_pDevice->GetVkDevice(), m_vkDescriptorPool, m_maxImages, m_descriptorSets.data());
	});
}

Program::Program(Program&& other) noexcept
	: m_pDevice(other.m_pDevice)
	, m_pSurface(other.m_pSurface)
	, m_pRenderPass(other.m_pRenderPass)
	, m_vkPipelineLayout(other.m_vkPipelineLayout)
	, m_vkPipeline(other.m_vkPipeline)
	, m_shaders(std::move(other.m_shaders))
	, m_maxImages(other.m_maxImages)
	, m_pVertexBuffer(other.m_pVertexBuffer)
	, m_vkDescriptorPool(other.m_vkDescriptorPool)
	, m_vkDescriptorSetLayout(other.m_vkDescriptorSetLayout)
{
	other.m_pDevice				  = nullptr;
	other.m_pSurface			  = nullptr;
	other.m_pRenderPass			  = nullptr;
	other.m_vkPipelineLayout	  = VK_NULL_HANDLE;
	other.m_vkPipeline			  = VK_NULL_HANDLE;
	other.m_maxImages			  = 0;
	other.m_pVertexBuffer		  = nullptr;
	other.m_vkDescriptorPool	  = VK_NULL_HANDLE;
	other.m_vkDescriptorSetLayout = VK_NULL_HANDLE;
}

Program::~Program()
{
	m_shaders.clear();
	m_releaseStack.ReleaseAll();
}

void Program::AttachShader(Shader&& shader)
{
	m_shaders.push_back(std::move(shader));
	NTT_ASSERT(m_shaders.back().GetVkShaderModule() != VK_NULL_HANDLE);
}

void Program::Link()
{
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
	inputAssemblyInfo.sType	   = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType								 = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount		 = 0;
	vertexInputInfo.vertexAttributeDescriptionCount		 = 0;

	VkPipelineRasterizationStateCreateInfo rasterizationInfo = {};
	rasterizationInfo.sType		  = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationInfo.cullMode	  = VK_CULL_MODE_NONE;
	rasterizationInfo.frontFace	  = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationInfo.lineWidth	  = 1.0f;

	VkPipelineTessellationStateCreateInfo tessellationInfo = {};
	tessellationInfo.sType								   = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;

	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
	};

	VkPipelineDynamicStateCreateInfo dynamicInfo = {};
	dynamicInfo.sType							 = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicInfo.dynamicStateCount				 = NTT_ARRAY_SIZE(dynamicStates);
	dynamicInfo.pDynamicStates					 = dynamicStates;

	VkPipelineMultisampleStateCreateInfo multisampleInfo = {};
	multisampleInfo.sType								 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleInfo.rasterizationSamples				 = VK_SAMPLE_COUNT_1_BIT;

	// u32 VkShaderStage
	u32									   shadersCount = u32(m_shaders.size());
	Array<VkPipelineShaderStageCreateInfo> stageInfos(shadersCount);

	for (u32 shaderIndex = 0u; shaderIndex < shadersCount; ++shaderIndex)
	{
		Shader&							 shader = m_shaders[shaderIndex];
		VkPipelineShaderStageCreateInfo& info	= stageInfos[shaderIndex];

		info.sType	= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		info.module = shader.GetVkShaderModule();
		info.pName	= "main";
		info.stage	= getShaderStageBitFromStageType(shader.GetShaderStage());
	}

	VkPipelineColorBlendAttachmentState attachment = {};
	attachment.alphaBlendOp						   = VK_BLEND_OP_ADD;
	attachment.blendEnable						   = VK_FALSE;
	attachment.colorBlendOp						   = VK_BLEND_OP_ADD;
	attachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo colorBlendStateInfo = {};
	colorBlendStateInfo.sType								= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendStateInfo.attachmentCount						= 1;
	colorBlendStateInfo.pAttachments						= &attachment;
	// colorBlendStateInfo.blendConstants = VK_BLEND_FACTOR_ONE;
	// colorBlendStateInfo.logicOp = ;

	VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = {};
	depthStencilStateInfo.sType			   = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilStateInfo.depthTestEnable  = VK_FALSE;
	depthStencilStateInfo.depthWriteEnable = VK_FALSE;

	Vec2	 size		   = m_pSurface->GetSize();
	VkRect2D scissors	   = {};
	scissors.extent.width  = u32(size.x);
	scissors.extent.height = u32(size.y);
	scissors.offset.x	   = 0;
	scissors.offset.y	   = 0;

	VkViewport viewport = {};
	viewport.width		= u32(size.x);
	viewport.height		= u32(size.y);
	viewport.x			= 0;
	viewport.y			= 0;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType								= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.scissorCount						= 1;
	viewportState.pScissors							= &scissors;
	viewportState.viewportCount						= 1;
	viewportState.pViewports						= &viewport;

	VkPipelineLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType					  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.setLayoutCount			  = 1;
	layoutInfo.pSetLayouts				  = &m_vkDescriptorSetLayout;

	vkCreatePipelineLayout(m_pDevice->GetVkDevice(), &layoutInfo, nullptr, &m_vkPipelineLayout);
	m_releaseStack.PushReleaseFunction(
		nullptr, [&](void*) { vkDestroyPipelineLayout(m_pDevice->GetVkDevice(), m_vkPipelineLayout, nullptr); });

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType						  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.basePipelineHandle			  = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex			  = -1;
	pipelineInfo.pInputAssemblyState		  = &inputAssemblyInfo;
	pipelineInfo.pVertexInputState			  = &vertexInputInfo;
	pipelineInfo.pRasterizationState		  = &rasterizationInfo;
	pipelineInfo.pTessellationState			  = &tessellationInfo;
	pipelineInfo.pDynamicState				  = &dynamicInfo;
	pipelineInfo.pMultisampleState			  = &multisampleInfo;
	pipelineInfo.pStages					  = stageInfos.data();
	pipelineInfo.stageCount					  = u32(stageInfos.size());
	pipelineInfo.pColorBlendState			  = &colorBlendStateInfo;
	pipelineInfo.pDepthStencilState			  = &depthStencilStateInfo;
	pipelineInfo.pViewportState				  = &viewportState;
	pipelineInfo.renderPass					  = m_pRenderPass->GetVkRenderPass();
	pipelineInfo.layout						  = m_vkPipelineLayout;

	VK_ASSERT(
		vkCreateGraphicsPipelines(m_pDevice->GetVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_vkPipeline));

	m_releaseStack.PushReleaseFunction(
		nullptr, [&](void*) { vkDestroyPipeline(m_pDevice->GetVkDevice(), m_vkPipeline, nullptr); });
}

void Program::Bind(CommandBuffer& commandBuffer)
{
	vkCmdBindPipeline(commandBuffer.GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_vkPipeline);

	vkCmdBindDescriptorSets(commandBuffer.GetVkCommandBuffer(),
							VK_PIPELINE_BIND_POINT_GRAPHICS,
							m_vkPipelineLayout,
							0,
							1,
							&m_descriptorSets[Renderer::GetCurrentFlight()],
							0,
							nullptr);

	IVec2 size = m_pSurface->GetSize();

	VkViewport viewport = {};
	viewport.x			= 0;
	viewport.y			= 0;
	viewport.width		= size.x;
	viewport.height		= size.y;
	viewport.minDepth	= 0.0f;
	viewport.maxDepth	= 1.0f;

	vkCmdSetViewport(commandBuffer.GetVkCommandBuffer(), 0, 1, &viewport);

	VkRect2D scissors	   = {};
	scissors.offset.x	   = 0;
	scissors.offset.y	   = 0;
	scissors.extent.width  = size.x;
	scissors.extent.height = size.y;

	vkCmdSetScissor(commandBuffer.GetVkCommandBuffer(), 0, 1, &scissors);
}

static VkShaderStageFlagBits getShaderStageBitFromStageType(ShaderStage type)
{
	switch (type)
	{
	case ShaderStage::SHADER_STAGE_VERTEX:
		return VK_SHADER_STAGE_VERTEX_BIT;
	case ShaderStage::SHADER_STAGE_TESSELLATION_CONTROL:
		return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	case ShaderStage::SHADER_STAGE_TESSELLATION_EVALUATION:
		return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	case ShaderStage::SHADER_STAGE_GEOMETRY:
		return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	case ShaderStage::SHADER_STAGE_FRAGMENT:
		return VK_SHADER_STAGE_FRAGMENT_BIT;
	case ShaderStage::SHADER_STAGE_COUNT:
		NTT_UNREACHABLE();
	}

	return VK_SHADER_STAGE_ALL;
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN