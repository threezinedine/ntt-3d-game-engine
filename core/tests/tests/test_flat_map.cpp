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
	EXPECT_EQ(map.get(42), 100);
}

TEST_F(FlatMapTest, GetNonExistedKey)
{
	FlatMap<int, int> map;

	map.insert(42, 100);

	EXPECT_THROW(map.get(12), std::exception);
}

TEST_F(FlatMapTest, InsertMultipleKeysWithDifferentGroup)
{
	FlatMap<s32, s32> map;

	map.insert(12, 33);
	map.insert(14, 23);

	EXPECT_TRUE(map.contains(12));
	EXPECT_TRUE(map.contains(14));
}

HashType autoGroup1Hash(const s32& key)
{
	return static_cast<HashType>((1 << 7) + (key & 0x7F));
}

TEST_F(FlatMapTest, InsertMultipleKeysWith1Group)
{
	FlatMap<s32, s32> map(nullptr, autoGroup1Hash);

	map.insert(34, 33);
	map.insert(35, 34);
	map.insert(36, 35);

	EXPECT_EQ(map.get(34), 33);
	EXPECT_EQ(map.get(35), 34);
	EXPECT_EQ(map.get(36), 35);
}

TEST_F(FlatMapTest, InsertMultipleKeysWith1GroupAndExceedSlotsCount)
{
	FlatMap<s32, s32> map(nullptr, autoGroup1Hash);

	for (s32 i = 0; i < NTT_SLOT_PER_GROUP; ++i)
	{
		map.insert(i, i + 100);
	}

	for (s32 i = 0; i < NTT_SLOT_PER_GROUP; ++i)
	{
		EXPECT_EQ(map.get(i), i + 100);
	}

	// Inserting one more should still work (will loop around to find empty slot)
	map.insert(NTT_SLOT_PER_GROUP, NTT_SLOT_PER_GROUP + 100);
	EXPECT_EQ(map.get(NTT_SLOT_PER_GROUP), NTT_SLOT_PER_GROUP + 100);
}

TEST_F(FlatMapTest, DeleteKeyAndStillNoError)
{
	FlatMap<int, int> map;

	map.insert(50, 200);

	EXPECT_TRUE(map.contains(50));
	EXPECT_EQ(map.get(50), 200);

	map.remove(50);
	EXPECT_FALSE(map.contains(50));
}

TEST_F(FlatMapTest, InsertMultipleKeysWithCollision)
{
	FlatMap<s32, s32> map(nullptr, autoGroup1Hash);

	map.insert(1, 10);
	map.insert(129, 20); // 129 will collide with 1 in control byte

	EXPECT_EQ(map.get(1), 10);
	EXPECT_EQ(map.get(129), 20);
}

TEST_F(FlatMapTest, InsertDuplicateKey)
{
	FlatMap<int, int> map;

	map.insert(50, 200);
	map.insert(50, 300); // Insert duplicate key

	EXPECT_TRUE(map.contains(50));
	EXPECT_EQ(map.get(50), 300); // Value should be updated to the latest
}

TEST_F(FlatMapTest, ReInsertSameDeletedKey)
{
	FlatMap<int, int> map;

	map.insert(75, 150);
	EXPECT_TRUE(map.contains(75));
	EXPECT_EQ(map.get(75), 150);

	map.remove(75);
	EXPECT_FALSE(map.contains(75));

	map.insert(75, 250); // Re-insert the same key
	EXPECT_TRUE(map.contains(75));
	EXPECT_EQ(map.get(75), 250);
}

TEST_F(FlatMapTest, Clear)
{
	FlatMap<int, int> map;

	map.insert(10, 100);
	map.insert(20, 200);
	map.insert(30, 300);

	EXPECT_TRUE(map.contains(10));
	EXPECT_TRUE(map.contains(20));
	EXPECT_TRUE(map.contains(30));

	map.clear();

	EXPECT_FALSE(map.contains(10));
	EXPECT_FALSE(map.contains(20));
	EXPECT_FALSE(map.contains(30));
}