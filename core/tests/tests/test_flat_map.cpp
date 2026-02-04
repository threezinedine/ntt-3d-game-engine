#include "test_common.h"

class FlatMapTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		SetupTesting();
	}

	void TearDown() override
	{
		TeardownTesting();
	}
};

TEST_F(FlatMapTest, EmptyMap)
{
	FlatMap<int, int> map;

	EXPECT_FALSE(map.contains(1));
}

TEST_F(FlatMapTest, InsertKey)
{
	FlatMap<int, int> map;

	map.insert(42, 100);

	EXPECT_TRUE(map.contains(42));
}

TEST_F(FlatMapTest, InsertMultipleKeysWithDifferentGroup)
{
	FlatMap<s32, s32> map;

	map.insert(12, 33);
	map.insert(14, 23);

	EXPECT_TRUE(map.contains(12));
	EXPECT_TRUE(map.contains(14));
}