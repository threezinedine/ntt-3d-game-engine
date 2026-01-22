#include "core/containers/string.h"
#include "core/memory/allocator.h"
#include "core/memory/memory_system.h"
#include "core/memory/memory_utils.h"
#include <cstring>

#define NTT_MAX_TEMP_BUFFER_SIZE 2048

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

u32 String::length() const
{
	return static_cast<u32>(strlen(m_pData));
}

static b8 isSubstringAt(const Char* str, const Char* substr, Size pos);

void String::replace(const String& search, const String& replace, b8 all)
{
	NTT_USE_ALLOCATOR(pAllocator, m_pAllocator);

	Char newBuffer[NTT_MAX_TEMP_BUFFER_SIZE];

	u32 currentLength = length();
	u32 searchLength  = search.length();
	u32 replaceLength = replace.length();

	u32 newBufferIndex = 0;
	u32 currentIndex   = 0; // Index for the original string

	while (currentIndex <= currentLength - searchLength)
	{
		if (isSubstringAt(m_pData, search.m_pData, currentIndex))
		{
			// Copy the replacement string into the new buffer
			MemoryCopy(&newBuffer[newBufferIndex], replace.m_pData, replaceLength * sizeof(Char));

			newBufferIndex += replaceLength;
			currentIndex += searchLength;

			if (!all)
			{
				break;
			}
		}
		else
		{
			newBuffer[newBufferIndex++] = m_pData[currentIndex++];
		}
	}

	MemoryCopy(&newBuffer[newBufferIndex], &m_pData[currentIndex], (currentLength - currentIndex) * sizeof(Char));
	newBufferIndex += (currentLength - currentIndex);

	newBuffer[newBufferIndex] = NTT_TERMINTED_CHAR;

	pAllocator->deallocate(m_pData, (currentLength + 1) * sizeof(Char));

	m_pData = (Char*)pAllocator->allocate((newBufferIndex + 1) * sizeof(Char));
	MemoryCopy(m_pData, newBuffer, (newBufferIndex + 1) * sizeof(Char));
}

static b8 isSubstringAt(const Char* str, const Char* substr, Size pos)
{
	if (str == nullptr || substr == nullptr)
	{
		return false;
	}

	if (pos + strlen(substr) > strlen(str))
	{
		return false;
	}

	Size substrLength = static_cast<Size>(strlen(substr));
	for (Size i = 0; i < substrLength; ++i)
	{
		if (str[pos + i] != substr[i])
		{
			return false;
		}
	}

	return true;
}

void print(const String& str, ConsoleColor color, ConsoleColor backgroundColor, bool bold)
{
	print(str.c_str(), color, backgroundColor, bold);
}

String toString(const String& value)
{
	return value;
}

} // namespace ntt
