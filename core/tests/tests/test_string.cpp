#include "test_common.h"

class StringTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		MemorySystem::Initialize();
	}

	void TearDown() override
	{
		MemorySystem::Shutdown();
	}
};

TEST_F(StringTest, ReplaceFunctionality)
{
	String testStr("The quick brown fox jumps over the lazy dog. The fox is quick.");

	// Replace all occurrences
	testStr.replace("fox", "cat", true);
	EXPECT_STREQ(testStr.c_str(), "The quick brown cat jumps over the lazy dog. The cat is quick.");

	// Replace first occurrence only
	testStr.replace("cat", "tiger", false);
	EXPECT_STREQ(testStr.c_str(), "The quick brown tiger jumps over the lazy dog. The cat is quick.");
}

TEST_F(StringTest, ReplaceNoOccurrences)
{
	String testStr("Hello, World!");

	// Try to replace a substring that doesn't exist
	testStr.replace("foo", "bar", true);
	EXPECT_STREQ(testStr.c_str(), "Hello, World!");
}

TEST_F(StringTest, FormatReplace)
{
	String testStr("Value: {}");
	testStr.replace("{}", "42", false);
	EXPECT_STREQ(testStr.c_str(), "Value: 42");
}

TEST_F(StringTest, TestLength)
{
	String testStr("Hello, World!");
	EXPECT_EQ(testStr.length(), 13u);

	String emptyStr;
	EXPECT_EQ(emptyStr.length(), 0u);
}