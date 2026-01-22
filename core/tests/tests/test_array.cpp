#include "test_common.h"

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

} // namespace

class ArrayTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		MemorySystem::Initialize();
		TestObject::resetCounters();
		NonMovableObject::resetCounters();
	}

	void TearDown() override
	{
		MemorySystem::Shutdown();
	}
};

TEST_F(ArrayTest, AppendAndAccessElements)
{
	Array<int> intArray;

	// Append elements
	intArray.append(10);
	intArray.append(20);
	intArray.append(30);

	// Check count
	EXPECT_EQ(intArray.count(), 3u);

	// Access elements
	EXPECT_EQ(intArray[0], 10);
	EXPECT_EQ(intArray[1], 20);
	EXPECT_EQ(intArray[2], 30);
}

TEST_F(ArrayTest, ConstructorsCount)
{
	Array<TestObject> objArray;
	objArray.append(TestObject());
	objArray.append(TestObject());
	objArray.append(TestObject());
	EXPECT_EQ(TestObject::constructorCount, 3u);
	EXPECT_EQ(TestObject::copyConstructorCount, 0u);
	EXPECT_EQ(TestObject::moveConstructorCount, 3u);
	EXPECT_EQ(TestObject::destructorCount, 3u); // Temporary objects destroyed after

	TestObject test;
	objArray.append(test);
	EXPECT_EQ(TestObject::constructorCount, 4u);
	EXPECT_EQ(TestObject::copyConstructorCount, 1u);
	EXPECT_EQ(TestObject::moveConstructorCount, 3u);
	EXPECT_EQ(TestObject::destructorCount, 3u); // Temporary objects destroyed after, the copyd object remains
}

TEST_F(ArrayTest, NonMovableObject)
{
	Array<NonMovableObject> objArray;
	NonMovableObject		nmo;

	objArray.append(nmo);
	objArray.append(nmo);
	objArray.append(nmo);

	EXPECT_EQ(NonMovableObject::constructorCount, 1u);
	EXPECT_EQ(NonMovableObject::copyConstructorCount, 3u);
	EXPECT_EQ(NonMovableObject::destructorCount, 0u); // No temporary objects destroyed after
}

TEST_F(ArrayTest, ClearFunctionality)
{
	Array<TestObject> testObjs;

	// Append elements
	testObjs.append(TestObject());
	testObjs.append(TestObject());
	testObjs.append(TestObject());

	EXPECT_EQ(testObjs.count(), 3u);
	EXPECT_EQ(TestObject::constructorCount, 3u);
	EXPECT_EQ(TestObject::destructorCount, 3u);

	testObjs.clear();
	EXPECT_EQ(testObjs.count(), 0u);
	EXPECT_EQ(TestObject::destructorCount, 6u); // All objects destroyed
}

TEST_F(ArrayTest, Destructor)
{
	{
		Array<TestObject> testObjs;

		// Append elements
		testObjs.append(TestObject());
		testObjs.append(TestObject());
		testObjs.append(TestObject());

		EXPECT_EQ(testObjs.count(), 3u);
		EXPECT_EQ(TestObject::constructorCount, 3u);
		EXPECT_EQ(TestObject::destructorCount, 3u);
	}

	// After going out of scope, all objects should be destroyed
	EXPECT_EQ(TestObject::destructorCount, 6u);
}

TEST_F(ArrayTest, Pointer)
{
	Array<TestObject*> ptrArray;
	TestObject		   obj1;

	ptrArray.append(&obj1);
	EXPECT_EQ(ptrArray.count(), 1u);

	EXPECT_NO_THROW(ptrArray.clear());

	EXPECT_EQ(TestObject::destructorCount, 0u); // Pointers are cleared, not the objects they point to
}