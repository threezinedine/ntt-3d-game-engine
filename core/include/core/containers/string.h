#pragma once
#include "pch.h"

namespace ntt {

class Allocator;

/**
 * Project string class for handling and manipulating strings.
 * With nullptr allocator, it will use the MemorySystem's default allocator.
 */
class String
{
public:
	String(Allocator* pStringAllocator = nullptr);					 // Empty string
	String(const char* cstr, Allocator* pStringAllocator = nullptr); // Copy to internal buffer

	/**
	 * Create a deep copy of another string
	 * @param other The other string to copy from
	 */
	String(const String& other); // Deep copy

	/**
	 * Free the other string's resources, so that the string will be invalid
	 *
	 * @param other The other string to move from
	 */
	String(String&& other) noexcept;

	~String();

public:
	/**
	 * Get the length of the string (number of characters, excluding null-terminator)
	 *
	 * @return The length of the string, not containing the null-terminator
	 */
	u32 length() const;

	/**
	 * Modify the current string with some replacements.
	 *
	 * @param search The substring to search for.
	 * @param replace The substring to replace with.
	 * @param all If true, replace all occurrences; if false, replace only the first occurrence.
	 */
	void replace(const String& search, const String& replace, b8 all = false);

public:
	const char* c_str() const
	{
		return m_pData;
	}

private:
	Allocator* m_pAllocator; /// The allocator used for memory management, can be nullptr, then use default allocator
	Char*	   m_pData;		 /// Pointer to the character data, always has the null-terminator
};

/**
 * Overloaded print function to print a String object to the console.
 */
void print(const String& str,
		   ConsoleColor	 color			 = CONSOLE_COLOR_DEFAULT,
		   ConsoleColor	 backgroundColor = CONSOLE_COLOR_DEFAULT,
		   bool			 bold			 = false);

#if 0
template <typename T, typename... Args>
String format(const T& first, const Args&... args)
{
	return String(); // Placeholder implementation
}
#endif

} // namespace ntt
