#pragma once

typedef unsigned char	   u8;
typedef unsigned short	   u16;
typedef unsigned int	   u32;
typedef unsigned long long u64;

typedef signed char		 s8;
typedef signed short	 s16;
typedef signed int		 s32;
typedef signed long long s64;

typedef float  f32;
typedef double f64;

typedef bool b8;
typedef u32	 b32;

typedef unsigned long Size;

typedef char Char;

#define NTT_KB 1024ULL
#define NTT_MB 1024ULL * NTT_KB
#define NTT_GB 1024ULL * NTT_MB

#define NTT_BIT(n) (1ULL << (n))

#define NTT_PI	3.14159265358979323846f
#define NTT_ESP 1e-5f

#define NTT_TERMINTED_CHAR '\0' /// The null-terminator character for strings

#define NTT_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0])) /// Get the number of elements in a static array
#define NTT_UNUSED(x)		(void)(x)						 /// To suppress unused variable warnings

#define _STRINGIFY(x) #x
#define STRINGIFY(x)  _STRINGIFY(x)

#if (defined(__GNUC__) || defined(__clang__)) && !defined(NTT_NO_ANALYZE)
#define NTT_BINDING			   __attribute__((annotate("binding")))
#define NTT_HIDDEN			   __attribute__((annotate("hidden")))
#define NTT_SINGLETON		   __attribute__((annotate("singleton")))
#define NTT_JSON			   __attribute__((annotate("json"))) NTT_BINDING
#define NTT_ALIAS(name)		   __attribute__((annotate("name_alias:" name)))
#define NTT_DEFAULT_VALUE(val) __attribute__((annotate("default_value:" #val)))
#else
#define NTT_BINDING
#define NTT_HIDDEN
#define NTT_SINGLETON
#define NTT_JSON
#define NTT_ALIAS(name)
#define NTT_DEFAULT_VALUE(val)
#endif

#define NTT_SINGLETON_DECLARE(class)                                                                                   \
public:                                                                                                                \
	static Scope<class>& GetInstance();                                                                                \
	static void			 Release() NTT_BINDING;                                                                        \
	class();                                                                                                           \
	NTT_DELETE_COPY(class)                                                                                             \
	NTT_DELETE_MOVE(class)                                                                                             \
	~class();                                                                                                          \
                                                                                                                       \
private:                                                                                                               \
	static Scope<class> s_instance;

#define NTT_SINGLETON_DEFINE(class)                                                                                    \
	Scope<class> class ::s_instance = NTT_NULLPTR;                                                                     \
	Scope<class>& class ::GetInstance()                                                                                \
	{                                                                                                                  \
		if (s_instance == NTT_NULLPTR)                                                                                 \
		{                                                                                                              \
			s_instance = CreateScope<class>();                                                                         \
		}                                                                                                              \
		return s_instance;                                                                                             \
	}                                                                                                                  \
	void class ::Release()                                                                                             \
	{                                                                                                                  \
		NTT_ASSERT(s_instance != NTT_NULLPTR);                                                                         \
		s_instance.reset();                                                                                            \
	}

#define NTT_JSON_DEFINE(structName)                                                                                    \
	struct structName;                                                                                                 \
	structName structName##FromJsonString(const String& jsonStr) NTT_BINDING;                                          \
	structName structName##FromJson(const Json& json);                                                                 \
	Json	   structName##ToJson(const structName& obj);                                                              \
	String	   structName##ToJsonString(const structName& obj) NTT_BINDING;

#include "platforms/console.h"
#include "platforms/debug.h"

#define NTT_ASSERT(condition)                                                                                          \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(condition))                                                                                              \
		{                                                                                                              \
			print("Assertion failed: " #condition, CONSOLE_COLOR_RED, CONSOLE_COLOR_DEFAULT, true);                    \
			ntt::debugBreak();                                                                                         \
		}                                                                                                              \
	} while (0)

#define NTT_ASSERT_MSG(condition, message)                                                                             \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(condition))                                                                                              \
		{                                                                                                              \
			print("Assertion failed: " #condition "\nMessage: " message,                                               \
				  CONSOLE_COLOR_RED,                                                                                   \
				  CONSOLE_COLOR_DEFAULT,                                                                               \
				  true);                                                                                               \
			ntt::debugBreak();                                                                                         \
		}                                                                                                              \
	} while (0)