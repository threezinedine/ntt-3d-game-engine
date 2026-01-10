#pragma once
#include <platforms/common.h>

/**
 * @file json_writer.h
 * Only be used for json writer testing purposes.
 */

namespace ntt {
NTT_JSON_DEFINE(TestVersion);

struct NTT_JSON TestVersion
{
	// clang-format off
	u32 major NTT_ALIAS("Major") NTT_DEFAULT_VALUE(1);
	u32 minor;
	u32 patch NTT_ALIAS("Patch") NTT_DEFAULT_VALUE(0);
	// clang-format on
};

} // namespace ntt