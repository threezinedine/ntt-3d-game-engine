#include "test_common.h"

TEST(TestJsonWriter, SerializeDeserializeProjectDescription)
{
	ProjectDescription originalDesc;
	originalDesc.name		   = "TestProject";
	originalDesc.version.major = 1;
	originalDesc.version.minor = 0;
	originalDesc.version.patch = 2;
	originalDesc.tags		   = {"game", "demo", "test"};

	Json			   json				= ProjectDescriptionToJson(originalDesc);
	ProjectDescription deserializedDesc = ProjectDescriptionFromJson(json);

	EXPECT_EQ(originalDesc.name, deserializedDesc.name);
	EXPECT_EQ(originalDesc.version.major, deserializedDesc.version.major);
	EXPECT_EQ(originalDesc.version.minor, deserializedDesc.version.minor);
	EXPECT_EQ(originalDesc.version.patch, deserializedDesc.version.patch);

	EXPECT_EQ(originalDesc.tags.size(), deserializedDesc.tags.size());
	for (size_t i = 0; i < originalDesc.tags.size(); ++i)
	{
		EXPECT_EQ(originalDesc.tags[i], deserializedDesc.tags[i]);
	}
}

TEST(TestJsonWriter, NormalTestVersion)
{
	TestVersion originalVersion;
	originalVersion.major = 2;
	originalVersion.minor = 1;
	originalVersion.patch = 3;

	Json		json	= TestVersionToJson(originalVersion);
	TestVersion version = TestVersionFromJson(json);

	EXPECT_EQ(originalVersion.major, version.major);
	EXPECT_EQ(originalVersion.minor, version.minor);
	EXPECT_EQ(originalVersion.patch, version.patch);
}

TEST(TestJsonWriter, FromString)
{
	String		jsonStr = R"({"Major":3,"minor":2,"Patch":1})";
	TestVersion version = TestVersionFromJsonString(jsonStr);

	EXPECT_EQ(version.major, 3);
	EXPECT_EQ(version.minor, 2);
	EXPECT_EQ(version.patch, 1);
}

TEST(TestJsonWriter, MissingFields)
{
	Json		json	= Json::parse(R"({"Major":2,"minor":5})");
	TestVersion version = TestVersionFromJson(json);

	EXPECT_EQ(version.major, 2);
	EXPECT_EQ(version.minor, 5);
	EXPECT_EQ(version.patch, 0);
}

TEST(TestJsonWriter, SkipDefaultValues)
{
	TestVersion originalVersion;
	originalVersion.major = 1; // Default value
	originalVersion.minor = 4;
	originalVersion.patch = 0; // Default value

	Json json = TestVersionToJson(originalVersion);
	EXPECT_TRUE(json.contains("minor"));
	EXPECT_FALSE(json.contains("Major")); // Should be skipped
	EXPECT_FALSE(json.contains("Patch")); // Should be skipped
}

TEST(TestJsonWriter, OtherType)
{
	TestSpectType originalSpect;
	originalSpect.typeA = 42;
	originalSpect.typeB = 3.14f; // Default value
	originalSpect.typeC = {1, 2, 3, 4, 5};
	originalSpect.typeD = "default_string"; // Default value

	Json		  json	= TestSpectTypeToJson(originalSpect);
	TestSpectType spect = TestSpectTypeFromJson(json);

	EXPECT_EQ(originalSpect.typeA, spect.typeA);
	EXPECT_EQ(originalSpect.typeB, spect.typeB);
	EXPECT_EQ(originalSpect.typeC.size(), spect.typeC.size());
	for (size_t i = 0; i < originalSpect.typeC.size(); ++i)
	{
		EXPECT_EQ(originalSpect.typeC[i], spect.typeC[i]);
	}
	EXPECT_EQ(originalSpect.typeD, spect.typeD);
}

TEST(TestJsonWriter, NestedStructs)
{
	NestedStruct originalNested;
	originalNested.version.major = 1;
	originalNested.version.minor = 2;
	originalNested.version.patch = 3;
	originalNested.versionList	 = {
		  {1, 0, 0},
		 {2, 1, 1},
		{3, 2, 2}
	 };

	Json		 json	= NestedStructToJson(originalNested);
	NestedStruct nested = NestedStructFromJson(json);

	EXPECT_EQ(originalNested.version.major, nested.version.major);
	EXPECT_EQ(originalNested.version.minor, nested.version.minor);
	EXPECT_EQ(originalNested.version.patch, nested.version.patch);

	EXPECT_EQ(originalNested.versionList.size(), nested.versionList.size());
	for (size_t i = 0; i < originalNested.versionList.size(); ++i)
	{
		EXPECT_EQ(originalNested.versionList[i].major, nested.versionList[i].major);
		EXPECT_EQ(originalNested.versionList[i].minor, nested.versionList[i].minor);
		EXPECT_EQ(originalNested.versionList[i].patch, nested.versionList[i].patch);
	}
}