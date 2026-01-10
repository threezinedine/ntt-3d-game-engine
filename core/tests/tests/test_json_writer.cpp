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