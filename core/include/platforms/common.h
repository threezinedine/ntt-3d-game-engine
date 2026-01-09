#pragma once

// =============== Typedef =============
typedef unsigned long long u64;
typedef unsigned int	   u32;
typedef unsigned short	   u16;
typedef unsigned char	   u8;

typedef signed long long i64;
typedef signed int		 i32;
typedef signed short	 i16;
typedef signed char		 i8;

typedef float  f32;
typedef double f64;

typedef bool b8;
typedef bool b32;
#define NTT_TRUE  true
#define NTT_FALSE false

#define NTT_NULLPTR nullptr

// =============== Constants =============
#define NTT_KB 1024ULL
#define NTT_MB 1024ULL * NTT_KB
#define NTT_GB 1024ULL * NTT_MB

#define NTT_PI	3.14159265358979323846f
#define NTT_ESP 1e-5f

// =============== Macros ===============
#define NTT_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0])) /// Get the number of elements in a static array
#define NTT_UNUSED(x)		(void)(x)						 /// To suppress unused variable warnings

#if NTT_DEBUG
#define NTT_ASSERT(cond)                                                                                               \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(cond))                                                                                                   \
		{                                                                                                              \
			Console::setColor(ConsoleColor::RED);                                                                      \
			Console::printf("Assertion Failed: %s, in %s at line %d", #cond, __FILE__, __LINE__);                      \
			debugBreak();                                                                                              \
		}                                                                                                              \
	} while (0)

#define NTT_ASSERT_MSG(cond, msg)                                                                                      \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(cond))                                                                                                   \
		{                                                                                                              \
			Console::setColor(ConsoleColor::RED);                                                                      \
			Console::printf("Assertion Failed: %s, in %s at line %d", msg, __FILE__, __LINE__);                        \
			debugBreak();                                                                                              \
		}                                                                                                              \
	} while (0)

#define NTT_UNREACHABLE()                                                                                              \
	do                                                                                                                 \
	{                                                                                                                  \
		Console::setColor(ConsoleColor::RED);                                                                          \
		Console::printf("Unreachable code reached in %s at line %d", __FILE__, __LINE__);                              \
		debugBreak();                                                                                                  \
	} while (0)

#define NTT_UNIMPLEMENTED()                                                                                            \
	do                                                                                                                 \
	{                                                                                                                  \
		Console::setColor(ConsoleColor::RED);                                                                          \
		Console::printf("Unimplemented code reached in %s at line %d", __FILE__, __LINE__);                            \
		debugBreak();                                                                                                  \
	} while (0)

#else
#define NTT_ASSERT(cond)
#define NTT_ASSERT_MSG(cond, msg)
#define NTT_UNREACHABLE()
#define NTT_UNIMPLEMENTED()
#endif

#if (defined(__GNUC__) || defined(__clang__)) && !defined(NTT_NO_ANALYZE)
#define NTT_BINDING __attribute__((annotate("binding")))
#define NTT_HIDDEN	__attribute__((annotate("hidden")))
#else
#define NTT_BINDING
#define NTT_HIDDEN
#endif

#include "console.h"
#include "trap.h"
