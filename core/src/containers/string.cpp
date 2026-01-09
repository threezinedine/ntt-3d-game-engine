#include "containers/string.h"

namespace ntt {
String::String()
	: m_data(nullptr)
{
}

String::String(const char* cstr)
	: m_data(nullptr)
{
}

String::String(const String& other)
	: m_data(nullptr)
{
}

String::String(String&& other) noexcept
{
}

String::~String()
{
}

} // namespace ntt
