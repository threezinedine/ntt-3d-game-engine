#include "graphics/types.h"
#include "platforms/common.h"
#include <filesystem>

namespace ntt {

ShaderStage getShaderStageFromFileName(const char* filename)
{
	std::filesystem::path filepath	= filename;
	String				  extension = filepath.extension();

	if (extension == ".vert")
	{
		return ShaderStage::SHADER_STAGE_VERTEX;
	}
	else if (extension == ".frag")
	{
		return ShaderStage::SHADER_STAGE_FRAGMENT;
	}

	NTT_UNREACHABLE();
	return ShaderStage::SHADER_STAGE_COUNT;
}

} // namespace ntt
