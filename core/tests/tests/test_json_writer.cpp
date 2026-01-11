#include "test_common.h"

TEST(TestJsonWriter, NormalTestVersion)
{
	VersionTest originalVersion;
	originalVersion.major = 2;
	originalVersion.minor = 1;
	originalVersion.patch = 3;

	Json		json	= TestVersionToJson(originalVersion);
	VersionTest version = TestVersionFromJson(json);

	EXPECT_EQ(originalVersion.major, version.major);
	EXPECT_EQ(originalVersion.minor, version.minor);
	EXPECT_EQ(originalVersion.patch, version.patch);
}

TEST(TestJsonWriter, FromString)
{
	String		jsonStr = R"({"Major":3,"minor":2,"Patch":1})";
	VersionTest version = TestVersionFromJsonString(jsonStr);

	EXPECT_EQ(version.major, 3);
	EXPECT_EQ(version.minor, 2);
	EXPECT_EQ(version.patch, 1);
}

TEST(TestJsonWriter, MissingFields)
{
	Json		json	= Json::parse(R"({"Major":2,"minor":5})");
	VersionTest version = TestVersionFromJson(json);

	EXPECT_EQ(version.major, 2);
	EXPECT_EQ(version.minor, 5);
	EXPECT_EQ(version.patch, 0);
}

TEST(TestJsonWriter, SkipDefaultValues)
{
	VersionTest originalVersion;
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
	SpectTypeTest originalSpect;
	originalSpect.typeA = 42;
	originalSpect.typeB = 3.14f; // Default value
	originalSpect.typeC = {1, 2, 3, 4, 5};
	originalSpect.typeD = "default_string"; // Default value
	originalSpect.typeE = true;				// Default value
	originalSpect.typeF = ntt::ID(1234567890);

	Json		  json	= TestSpectTypeToJson(originalSpect);
	SpectTypeTest spect = TestSpectTypeFromJson(json);

	EXPECT_EQ(originalSpect.typeA, spect.typeA);
	EXPECT_EQ(originalSpect.typeB, spect.typeB);
	EXPECT_EQ(originalSpect.typeC.size(), spect.typeC.size());
	for (size_t i = 0; i < originalSpect.typeC.size(); ++i)
	{
		EXPECT_EQ(originalSpect.typeC[i], spect.typeC[i]);
	}
	EXPECT_EQ(originalSpect.typeD, spect.typeD);
}

TEST(TestJsonWriter, OtherTypeDefaultValues)
{
	SpectTypeTest originalSpect;
	originalSpect.typeA = 7;
	originalSpect.typeB = 3.14f; // Default value
	originalSpect.typeC = {10, 20, 30};
	originalSpect.typeD = "default_string";				// Default value
	originalSpect.typeE = true;							// Default value
	originalSpect.typeF = ntt::ID(ntt::INVALID_ID_RAW); // Default value

	Json json = TestSpectTypeToJson(originalSpect);
	EXPECT_TRUE(json.contains("typeA"));
	EXPECT_TRUE(json.contains("typeC"));
	EXPECT_FALSE(json.contains("typeB")); // Should be skipped
	EXPECT_FALSE(json.contains("typeD")); // Should be skipped
	EXPECT_FALSE(json.contains("typeE")); // Should be skipped
	EXPECT_FALSE(json.contains("typeF")); // Should be skipped
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

TEST(TestJsonWriter, GlmType)
{
	GlmTest originalGlm;
	originalGlm.position = Vec2(1.0f, 2.0f);

	Json	json	= GlmTestToJson(originalGlm);
	GlmTest glmTest = GlmTestFromJson(json);

	EXPECT_EQ(originalGlm.position.x, glmTest.position.x);
	EXPECT_EQ(originalGlm.position.y, glmTest.position.y);
}

TEST(TestJsonWriter, GlmTypeDefaultValue)
{
	GlmTest originalGlm;
	originalGlm.position = Vec2(0.0f, 1.0f); // Default value

	Json json = GlmTestToJson(originalGlm);
	EXPECT_FALSE(json.contains("position")); // Should be skipped
}

TEST(TestJsonWriter, MissingGlmType)
{
	Json	json	= Json::parse(R"({})");
	GlmTest glmTest = GlmTestFromJson(json);

	EXPECT_EQ(glmTest.position.x, 0.0f);
	EXPECT_EQ(glmTest.position.y, 1.0f);
}