#include "core/containers/string.h"
#include "core/memory/allocator.h"
#include "core/memory/memory_system.h"
#include "core/memory/memory_utils.h"
#include <cstring>

namespace ntt {

String::String(Allocator* pStringAllocator)
	: m_pAllocator(pStringAllocator)
{
	NTT_USE_ALLOCATOR(pAllocator, m_pAllocator);

	m_pData	   = (Char*)pAllocator->allocate(1 * sizeof(Char));
	m_pData[0] = NTT_TERMINTED_CHAR;
}

String::String(const char* cstr, Allocator* pStringAllocator)
	: m_pAllocator(pStringAllocator)
{
	NTT_USE_ALLOCATOR(pAllocator, m_pAllocator);

	Size length = static_cast<Size>(strlen(cstr));
	m_pData		= (Char*)pAllocator->allocate((length + 1) * sizeof(Char));
	MemoryCopy(m_pData, cstr, length * sizeof(Char));
	m_pData[length] = NTT_TERMINTED_CHAR;
}

String::String(const String& other)
	: m_pAllocator(other.m_pAllocator)
{
	NTT_USE_ALLOCATOR(pAllocator, m_pAllocator);

	Size length = static_cast<Size>(strlen(other.m_pData));
	m_pData		= (Char*)pAllocator->allocate((length + 1) * sizeof(Char));
	MemoryCopy(m_pData, other.m_pData, (length + 1) * sizeof(Char));
}

String::String(String&& other) noexcept
	: m_pAllocator(other.m_pAllocator)
{
	other.m_pAllocator = nullptr;

	NTT_USE_ALLOCATOR(pAllocator, m_pAllocator);

	m_pData = (Char*)pAllocator->allocate((strlen(other.m_pData) + 1) * sizeof(Char));
	MemoryCopy(m_pData, other.m_pData, (strlen(other.m_pData) + 1) * sizeof(Char));
	pAllocator->deallocate(other.m_pData, (strlen(other.m_pData) + 1) * sizeof(Char));
	other.m_pData = nullptr;
}

String::~String()
{
	NTT_USE_ALLOCATOR(pAllocator, m_pAllocator);

	if (m_pData != nullptr)
	{
		pAllocator->deallocate(m_pData, (strlen(m_pData) + 1) * sizeof(Char));
		m_pData = nullptr;
	}
}

void print(const String& str, ConsoleColor color, ConsoleColor backgroundColor, bool bold)
{
	print(str.c_str(), color, backgroundColor, bold);
}

} // namespace ntt
