#pragma once
#include "platforms/common.h"

namespace ntt {

NTT_JSON_DEFINE(Version)
struct NTT_JSON Version
{
	u32 major NTT_DEFAULT_VALUE(1);
	u32 minor NTT_DEFAULT_VALUE(0);
	u32 patch NTT_DEFAULT_VALUE(0);
};

NTT_JSON_DEFINE(ProjectDescription)

struct NTT_JSON ProjectDescription
{
	Version version;
	String	name;
};

} // namespace ntt
