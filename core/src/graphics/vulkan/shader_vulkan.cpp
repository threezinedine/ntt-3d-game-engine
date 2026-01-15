#if NTT_USE_GRAPHICS_VULKAN

#include "graphics/vulkan/shader_vulkan.h"
#include "containers/release_stack.h"
#include "graphics/vulkan/vulkan_device.h"
#include <filesystem>
#include <fstream>
#include <glslang/Include/glslang_c_interface.h>
#include <glslang/Public/resource_limits_c.h>

namespace ntt {

struct SpirVBinary
{
	u32* words;
	i32	 size;
};

static ShaderStage	   getShaderStageFromGLSLangStage(glslang_stage_t stage);
static glslang_stage_t getGLSLangStageFromShaderStage(ShaderStage stage);
static glslang_stage_t getGLSLangStageFromFileName(const char* filename);

Shader::Shader(Device* pDevice, const char* filename)
	: m_pDevice(pDevice)
	, m_vkShaderModule(VK_NULL_HANDLE)
	, m_isValid(false)
{
	glslang_stage_t stage = getGLSLangStageFromFileName(filename);
	m_stage				  = getShaderStageFromGLSLangStage(stage);

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

	NTT_VULKAN_LOG_DEBUG("Shader \"%s\" is loaded", m_filename);
}

Shader::Shader(Shader&& other)
	: m_pDevice(other.m_pDevice)
	, m_stage(other.m_stage)
	, m_vkShaderModule(other.m_vkShaderModule)
	, m_isValid(other.m_isValid)
	, m_releaseStack(std::move(other.m_releaseStack))
{
	other.m_stage		   = ShaderStage::SHADER_STAGE_COUNT;
	other.m_vkShaderModule = VK_NULL_HANDLE;
	other.m_pDevice		   = nullptr;
	other.m_isValid		   = false;
}

Shader::~Shader()
{
	if (m_vkShaderModule != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(m_pDevice->GetVkDevice(), m_vkShaderModule, nullptr);
		NTT_VULKAN_LOG_DEBUG("Shader \"%s\" is destroyed", m_filename);
	}
}

void Shader::Compile()
{
	glslang_stage_t stage = getGLSLangStageFromShaderStage(m_stage);

	glslang_input_t input					= {};
	input.language							= GLSLANG_SOURCE_GLSL;
	input.stage								= stage;
	input.client							= GLSLANG_CLIENT_VULKAN;
	input.client_version					= GLSLANG_TARGET_VULKAN_1_0;
	input.target_language					= GLSLANG_TARGET_SPV;
	input.target_language_version			= GLSLANG_TARGET_SPV_1_0;
	input.code								= m_content.c_str();
	input.default_version					= 100;
	input.default_profile					= GLSLANG_NO_PROFILE;
	input.force_default_version_and_profile = false;
	input.forward_compatible				= false;
	input.messages							= GLSLANG_MSG_DEFAULT_BIT;
	input.resource							= glslang_default_resource();

	glslang_shader_t* shader = glslang_shader_create(&input);
	m_releaseStack.PushReleaseFunction(shader,
									   [&](void* pUserData) { glslang_shader_delete((glslang_shader_t*)(pUserData)); });

	SpirVBinary bin = {
		.words = NULL,
		.size  = 0,
	};

	if (!glslang_shader_preprocess(shader, &input))
	{
		NTT_VULKAN_LOG_ERROR("GLSL preprocessing failed %s", m_filename);
		NTT_VULKAN_LOG_ERROR("\t%s", glslang_shader_get_info_log(shader));
		NTT_VULKAN_LOG_ERROR("\t%s", glslang_shader_get_info_debug_log(shader));
		NTT_VULKAN_LOG_ERROR("%s", input.code);

		m_releaseStack.ReleaseAll();
		return;
	}

	if (!glslang_shader_parse(shader, &input))
	{
		NTT_VULKAN_LOG_ERROR("GLSL parsing failed %s", m_filename);
		NTT_VULKAN_LOG_ERROR("\t%s", glslang_shader_get_info_log(shader));
		NTT_VULKAN_LOG_ERROR("\t%s", glslang_shader_get_info_debug_log(shader));

		m_releaseStack.ReleaseAll();
		return;
	}

	glslang_program_t* program = glslang_program_create();
	m_releaseStack.PushReleaseFunction(program,
									   [&](void* pUserData) { glslang_program_delete((glslang_program_t*)pUserData); });

	glslang_program_add_shader(program, shader);

	if (!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT))
	{
		NTT_VULKAN_LOG_ERROR("GLSL linking failed %s", m_filename);
		NTT_VULKAN_LOG_ERROR("\t%s", glslang_program_get_info_log(program));
		NTT_VULKAN_LOG_ERROR("\t%s", glslang_program_get_info_debug_log(program));

		m_releaseStack.ReleaseAll();
		return;
	}

	glslang_program_SPIRV_generate(program, stage);

	bin.size  = glslang_program_SPIRV_get_size(program);
	bin.words = (u32*)malloc(bin.size * sizeof(u32));
	glslang_program_SPIRV_get(program, bin.words);

	const char* spirv_messages = glslang_program_SPIRV_get_messages(program);
	if (spirv_messages)
	{
		NTT_VULKAN_LOG_ERROR("(%s) %s", m_filename, spirv_messages);
		m_releaseStack.ReleaseAll();
		return;
	}

	m_releaseStack.ReleaseAll();

	VkShaderModuleCreateInfo shaderInfo = {};
	shaderInfo.sType					= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderInfo.codeSize					= bin.size * sizeof(u32);
	shaderInfo.pCode					= bin.words;

	VK_ASSERT(vkCreateShaderModule(m_pDevice->GetVkDevice(), &shaderInfo, nullptr, &m_vkShaderModule));

	NTT_VULKAN_LOG_DEBUG("Shader %s is compiled", m_filename);

	m_isValid = true;

	return;
}

static ShaderStage getShaderStageFromGLSLangStage(glslang_stage_t stage)
{
	switch (stage)
	{
	case GLSLANG_STAGE_VERTEX:
		return ShaderStage::SHADER_STAGE_VERTEX;
	case GLSLANG_STAGE_TESSCONTROL:
		return ShaderStage::SHADER_STAGE_TESSELLATION_CONTROL;
	case GLSLANG_STAGE_TESSEVALUATION:
		return ShaderStage::SHADER_STAGE_TESSELLATION_EVALUATION;
	case GLSLANG_STAGE_GEOMETRY:
		return ShaderStage::SHADER_STAGE_GEOMETRY;
	case GLSLANG_STAGE_FRAGMENT:
		return ShaderStage::SHADER_STAGE_FRAGMENT;
	default:
		return ShaderStage::SHADER_STAGE_COUNT;
	}
}

static glslang_stage_t getGLSLangStageFromShaderStage(ShaderStage stage)
{
	switch (stage)
	{
	case ShaderStage::SHADER_STAGE_VERTEX:
		return GLSLANG_STAGE_VERTEX;
	case ShaderStage::SHADER_STAGE_TESSELLATION_CONTROL:
		return GLSLANG_STAGE_TESSCONTROL;
	case ShaderStage::SHADER_STAGE_TESSELLATION_EVALUATION:
		return GLSLANG_STAGE_TESSEVALUATION;
	case ShaderStage::SHADER_STAGE_GEOMETRY:
		return GLSLANG_STAGE_GEOMETRY;
	case ShaderStage::SHADER_STAGE_FRAGMENT:
		return GLSLANG_STAGE_FRAGMENT;
	default:
		return GLSLANG_STAGE_COUNT;
	}
}

static glslang_stage_t getGLSLangStageFromFileName(const char* filename)
{
	std::filesystem::path filepath	= filename;
	String				  extension = filepath.extension();

	if (extension == ".vert")
	{
		return GLSLANG_STAGE_VERTEX;
	}
	else if (extension == ".frag")
	{
		return GLSLANG_STAGE_FRAGMENT;
	}

	NTT_UNREACHABLE();
	return GLSLANG_STAGE_COUNT;
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN