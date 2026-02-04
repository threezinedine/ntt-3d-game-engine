#pragma once
#include "engine/engine.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ntt;
using namespace ::testing;

namespace {

class NonMovableObject
{
public:
	NonMovableObject()
	{
		NonMovableObject::constructorCount++;
	}

	NonMovableObject(const NonMovableObject& other)
	{
		NTT_UNUSED(other);
		NonMovableObject::copyConstructorCount++;
	}

	NonMovableObject(NonMovableObject&& other) noexcept = delete;

	~NonMovableObject()
	{
		NonMovableObject::destructorCount++;
	}

public:
	static inline void resetCounters()
	{
		constructorCount	 = 0;
		copyConstructorCount = 0;
		destructorCount		 = 0;
	}

	static inline u32 constructorCount	   = 0;
	static inline u32 copyConstructorCount = 0;
	static inline u32 destructorCount	   = 0;
};

class TestObject
{
public:
	TestObject()
	{
		TestObject::constructorCount++;
	}

	TestObject(const TestObject& other)
	{
		NTT_UNUSED(other);
		TestObject::copyConstructorCount++;
	}

	TestObject(TestObject&& other) noexcept
	{
		NTT_UNUSED(other);
		TestObject::moveConstructorCount++;
	}

	~TestObject()
	{
		TestObject::destructorCount++;
	}

public:
	static inline void resetCounters()
	{
		constructorCount	 = 0;
		copyConstructorCount = 0;
		moveConstructorCount = 0;
		destructorCount		 = 0;
	}

	static inline u32 constructorCount	   = 0;
	static inline u32 copyConstructorCount = 0;
	static inline u32 moveConstructorCount = 0;
	static inline u32 destructorCount	   = 0;
};

void SetupTesting()
{
	MemorySystem::Initialize();
	TestObject::resetCounters();
	NonMovableObject::resetCounters();
}

void TeardownTesting()
{
	MemorySystem::Shutdown();
}

} // namespace
