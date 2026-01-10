#pragma once
#include <cstddef>
#include <cstring>
#include <functional>
#include <glm/glm.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

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

template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args&&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

#if NTT_NO_ANALYZE
template <typename T>
using Array = std::vector<T>;
#else
template <typename T>
class Array;
#endif

using String = std::string;

typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;

typedef glm::ivec2 IVec2;
typedef glm::ivec3 IVec3;
typedef glm::ivec4 IVec4;

typedef glm::mat3 Mat3;
typedef glm::mat4 Mat4;

typedef nlohmann::json Json;

// =============== Constants =============
#define NTT_KB 1024ULL
#define NTT_MB 1024ULL * NTT_KB
#define NTT_GB 1024ULL * NTT_MB

#define NTT_BIT(n) (1ULL << (n))

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
			Console::setColor(CONSOLE_COLOR_RED);                                                                      \
			Console::printf("Assertion Failed: %s, in %s at line %d", #cond, __FILE__, __LINE__);                      \
			debugBreak();                                                                                              \
		}                                                                                                              \
	} while (0)

#define NTT_ASSERT_MSG(cond, msg)                                                                                      \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(cond))                                                                                                   \
		{                                                                                                              \
			Console::setColor(CONSOLE_COLOR_RED);                                                                      \
			Console::printf("Assertion Failed: %s, in %s at line %d", msg, __FILE__, __LINE__);                        \
			debugBreak();                                                                                              \
		}                                                                                                              \
	} while (0)

#define NTT_UNREACHABLE()                                                                                              \
	do                                                                                                                 \
	{                                                                                                                  \
		Console::setColor(CONSOLE_COLOR_RED);                                                                          \
		Console::printf("Unreachable code reached in %s at line %d", __FILE__, __LINE__);                              \
		debugBreak();                                                                                                  \
	} while (0)

#define NTT_UNIMPLEMENTED()                                                                                            \
	do                                                                                                                 \
	{                                                                                                                  \
		Console::setColor(CONSOLE_COLOR_RED);                                                                          \
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

#include "console.h"
#include "trap.h"

#define NTT_DELETE_COPY(class)                                                                                         \
	class(const class&)			   = delete;                                                                           \
	class& operator=(const class&) = delete;

#define NTT_DELETE_MOVE(class)                                                                                         \
	class(class&&)			  = delete;                                                                                \
	class& operator=(class&&) = delete;

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