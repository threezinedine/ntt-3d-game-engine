#pragma once
#include <platforms/common.h>

/**
 * @file json_writer.h
 * Only be used for json writer testing purposes.
 */

#define NTT_JSON_PREMIVE_TYPE(type)                                                                                    \
	Json type##ToJson(const type& value);                                                                              \
	type type##FromJson(const Json& json);

NTT_JSON_PREMIVE_TYPE(u64)
NTT_JSON_PREMIVE_TYPE(u32)
NTT_JSON_PREMIVE_TYPE(u16)
NTT_JSON_PREMIVE_TYPE(u8)
NTT_JSON_PREMIVE_TYPE(i64)
NTT_JSON_PREMIVE_TYPE(i32)
NTT_JSON_PREMIVE_TYPE(i16)
NTT_JSON_PREMIVE_TYPE(i8)
NTT_JSON_PREMIVE_TYPE(f32)
NTT_JSON_PREMIVE_TYPE(f64)
NTT_JSON_PREMIVE_TYPE(b8)
NTT_JSON_PREMIVE_TYPE(String)
NTT_JSON_PREMIVE_TYPE(Vec2)
NTT_JSON_PREMIVE_TYPE(Vec3)
NTT_JSON_PREMIVE_TYPE(Vec4)
NTT_JSON_PREMIVE_TYPE(IVec2)
NTT_JSON_PREMIVE_TYPE(IVec3)
NTT_JSON_PREMIVE_TYPE(IVec4)

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

NTT_JSON_DEFINE(TestSpectType);
struct NTT_JSON TestSpectType
{
	// clang-format off
	u32		   typeA;
	f32        typeB  NTT_DEFAULT_VALUE(3.14);
	Array<i16> typeC;
	String     typeD  NTT_DEFAULT_VALUE("default_string");
	b8         typeE  NTT_DEFAULT_VALUE(true);
	// clang-format on
};

NTT_JSON_DEFINE(NestedStruct);
struct NTT_JSON NestedStruct
{
	TestVersion		   version;
	Array<TestVersion> versionList;
};

NTT_JSON_DEFINE(GlmTest);
struct NTT_JSON GlmTest
{
	Vec2 position NTT_DEFAULT_VALUE(Vec2(0.0f, 1.0f));
};

} // namespace ntt