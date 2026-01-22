#pragma once

#include "pch.h"
#include "string.h"

namespace ntt {

class Allocator;

/**
 * The containers module includes all the commonly used data structures such as Array, String, etc.
 * These data structures are designed to be efficient and easy to use, providing essential
 * functionality for managing collections of data within the engine. Using the JavaScript Object Notation
 * (JSON) format, complex data can be easily represented and manipulated.
 */
class Json
{
public:
	/**
	 * All possible value types in a JSON structure.
	 */
	union Value {
		s8	   s8Value;
		u8	   u8Value;
		s16	   s16Value;
		u16	   u16Value;
		s32	   s32Value;
		u32	   u32Value;
		s64	   s64Value;
		u64	   u64Value;
		f32	   f32Value;
		f64	   f64Value;
		b8	   b8Value;
		String stringValue;
	};

public:
	Json(Allocator* pJsonAllocator = nullptr);
	Json(const Json& other);
	Json(Json&& other) noexcept;
	~Json();

public:
	/**
	 * String representation of the JSON object. Can configure indentation level.
	 *
	 * @param indentLevel The number of spaces to use for indentation. If 0 (default), no indentation is applied, no
	 *      new lines are added.
	 */
	String toString(u32 indentLevel = 0) const;

private:
	Allocator* m_pAllocator; /// The allocator used for memory management, can be nullptr,
};

} // namespace ntt
