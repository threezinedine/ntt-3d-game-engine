#include "test_common.h"

TEST(IDTest, DefaultConstructorCreatesInvalidID)
{
	ID id1;
	ID id2;

	EXPECT_FALSE(id1.IsValid());
}

TEST(IDTest, RawConstructorCreatesValidID)
{
	IDRawType rawId = u32(0x14);
	ID		  id(rawId);

	EXPECT_TRUE(id.IsValid());
	EXPECT_EQ(static_cast<IDRawType>(id), rawId);
}

TEST(IDTest, CopyConstructorCopiesID)
{
	IDRawType rawId = u32(0x25);
	ID		  originalId(rawId);
	ID		  copiedId(originalId);

	EXPECT_TRUE(copiedId.IsValid());
	EXPECT_EQ(static_cast<IDRawType>(copiedId), rawId);
}

TEST(IDTest, MoveConstructorMovesID)
{
	IDRawType rawId = u32(0x36);
	ID		  originalId(rawId);
	ID		  movedId(std::move(originalId));

	EXPECT_TRUE(movedId.IsValid());
	EXPECT_EQ(static_cast<IDRawType>(movedId), rawId);
	EXPECT_FALSE(originalId.IsValid());
}

TEST(IDTest, CheckEqualityOperator)
{
	IDRawType rawId1 = u32(0x47);
	IDRawType rawId2 = u32(0x58);
	ID		  id1(rawId1);
	ID		  id2(rawId1);
	ID		  id3(rawId2);
	ID		  id4(0x01, 0, 0x47); // Example parameters

	EXPECT_TRUE(id1 == id2);
	EXPECT_FALSE(id1 != id2);
	EXPECT_FALSE(id1 == id3);
	EXPECT_TRUE(id1 != id3);
	EXPECT_TRUE(id1 == id4);
	EXPECT_FALSE(id3 == id4);
}

TEST(IDTest, CheckLatestVersion)
{
	Scope<IDManager>& idManager = IDManager::GetInstance();

	ID id1 = idManager->CreateID(IDType::ID_TYPE_ENTITY);
	ID id2 = id1;

	EXPECT_TRUE(id1.IsLatest());
	EXPECT_TRUE(id2.IsLatest());

	id1.Update();

	EXPECT_TRUE(id1.IsLatest());
	EXPECT_FALSE(id2.IsLatest());

	id2.Update(); // do nothing
	EXPECT_TRUE(id1.IsLatest());
	EXPECT_FALSE(id2.IsLatest());
}