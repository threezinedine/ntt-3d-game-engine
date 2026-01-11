#include <json_writer/json_writer.h>
#include <utils/id/id.h>

#define NTT_JSON_PREMIVE_TYPE_IMPL(type, jsonType)                                                                     \
	Json type##ToJson(const type& value)                                                                               \
	{                                                                                                                  \
		return Json(value);                                                                                            \
	}                                                                                                                  \
                                                                                                                       \
	type type##FromJson(const Json& json)                                                                              \
	{                                                                                                                  \
		return json.get<jsonType>();                                                                                   \
	}

#define NTT_NORMAL_CHECK_DEFAULT(type)                                                                                 \
	b8 type##IsEqual(const type& value, const type& defaultValue)                                                      \
	{                                                                                                                  \
		return value == defaultValue;                                                                                  \
	}

#define NTT_FLOAT_CHECK_DEFAULT(type)                                                                                  \
	b8 type##IsEqual(const type& value, const type& defaultValue)                                                      \
	{                                                                                                                  \
		return std::abs(value - defaultValue) < NTT_ESP;                                                               \
	}

NTT_JSON_PREMIVE_TYPE_IMPL(u64, u64)
NTT_NORMAL_CHECK_DEFAULT(u64)

NTT_JSON_PREMIVE_TYPE_IMPL(u32, u32)
NTT_NORMAL_CHECK_DEFAULT(u32)

NTT_JSON_PREMIVE_TYPE_IMPL(u16, u16)
NTT_NORMAL_CHECK_DEFAULT(u16)

NTT_JSON_PREMIVE_TYPE_IMPL(u8, u8)
NTT_NORMAL_CHECK_DEFAULT(u8)

NTT_JSON_PREMIVE_TYPE_IMPL(i64, i64)
NTT_NORMAL_CHECK_DEFAULT(i64)

NTT_JSON_PREMIVE_TYPE_IMPL(i32, i32)
NTT_NORMAL_CHECK_DEFAULT(i32)

NTT_JSON_PREMIVE_TYPE_IMPL(i16, i16)
NTT_NORMAL_CHECK_DEFAULT(i16)

NTT_JSON_PREMIVE_TYPE_IMPL(i8, i8)
NTT_NORMAL_CHECK_DEFAULT(i8)

NTT_JSON_PREMIVE_TYPE_IMPL(f32, f32)
NTT_FLOAT_CHECK_DEFAULT(f32)

NTT_JSON_PREMIVE_TYPE_IMPL(f64, f64)
NTT_FLOAT_CHECK_DEFAULT(f64)

NTT_JSON_PREMIVE_TYPE_IMPL(b8, bool)
NTT_NORMAL_CHECK_DEFAULT(b8)

NTT_JSON_PREMIVE_TYPE_IMPL(String, String)
NTT_NORMAL_CHECK_DEFAULT(String)

Json Vec2ToJson(const Vec2& value)
{
	return Json::array({value.x, value.y});
}

Vec2 Vec2FromJson(const Json& json)
{
	Vec2 value;
	if (json.is_array() && json.size() == 2)
	{
		value.x = json.at(0).get<f32>();
		value.y = json.at(1).get<f32>();
	}
	return value;
}

NTT_NORMAL_CHECK_DEFAULT(Vec2)

Json Vec3ToJson(const Vec3& value)
{
	return Json::array({value.x, value.y, value.z});
}

Vec3 Vec3FromJson(const Json& json)
{
	Vec3 value;
	if (json.is_array() && json.size() == 3)
	{
		value.x = json.at(0).get<f32>();
		value.y = json.at(1).get<f32>();
		value.z = json.at(2).get<f32>();
	}
	return value;
}

NTT_NORMAL_CHECK_DEFAULT(Vec3)

Json Vec4ToJson(const Vec4& value)
{
	return Json::array({value.x, value.y, value.z, value.w});
}

Vec4 Vec4FromJson(const Json& json)
{
	Vec4 value;
	if (json.is_array() && json.size() == 4)
	{
		value.x = json.at(0).get<f32>();
		value.y = json.at(1).get<f32>();
		value.z = json.at(2).get<f32>();
		value.w = json.at(3).get<f32>();
	}
	return value;
}

NTT_NORMAL_CHECK_DEFAULT(Vec4)

Json IVec2ToJson(const IVec2& value)
{
	return Json::array({value.x, value.y});
}

IVec2 IVec2FromJson(const Json& json)
{
	IVec2 value;
	if (json.is_array() && json.size() == 2)
	{
		value.x = json.at(0).get<i32>();
		value.y = json.at(1).get<i32>();
	}
	return value;
}

NTT_NORMAL_CHECK_DEFAULT(IVec2)

Json IVec3ToJson(const IVec3& value)
{
	return Json::array({value.x, value.y, value.z});
}

IVec3 IVec3FromJson(const Json& json)
{
	IVec3 value;
	if (json.is_array() && json.size() == 3)
	{
		value.x = json.at(0).get<i32>();
		value.y = json.at(1).get<i32>();
		value.z = json.at(2).get<i32>();
	}
	return value;
}

NTT_NORMAL_CHECK_DEFAULT(IVec3)

Json IVec4ToJson(const IVec4& value)
{
	return Json::array({value.x, value.y, value.z, value.w});
}

IVec4 IVec4FromJson(const Json& json)
{
	IVec4 value;
	if (json.is_array() && json.size() == 4)
	{
		value.x = json.at(0).get<i32>();
		value.y = json.at(1).get<i32>();
		value.z = json.at(2).get<i32>();
		value.w = json.at(3).get<i32>();
	}
	return value;
}

NTT_NORMAL_CHECK_DEFAULT(IVec4)

namespace ntt {

Json IDToJson(const ID& value)
{
	return Json(value.ToRaw());
}

ID IDFromJson(const Json& json)
{
	return ntt::ID(json.get<u64>());
}

NTT_NORMAL_CHECK_DEFAULT(ID)

} // namespace ntt
