#include "core/containers/json.h"

namespace ntt {

Json::Json(Allocator* pJsonAllocator)
	: m_pAllocator(pJsonAllocator)
{
}

Json::Json(const Json& other)
	: m_pAllocator(other.m_pAllocator)
{
}

Json::Json(Json&& other) noexcept
	: m_pAllocator(other.m_pAllocator)
{
	other.m_pAllocator = nullptr;
}

Json::~Json()
{
}

String Json::toString(u32 indentLevel) const
{
	NTT_UNUSED(indentLevel);
	return String();
}

} // namespace ntt
