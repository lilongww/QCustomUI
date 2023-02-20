#include <OpenVisa/Object.h>
#include <OpenVisa/Utils.h>
#include <OpenVisa/Version.h>
#include <gtest/gtest.h>

enum class EnumTest
{
    MLinear,
    MLogArithmic,
    Phase
};

namespace OpenVisa
{
VISA_ENUM_ADL_DECLARE(EnumTest::MLinear, EnumTest::Phase, "MLIN", "MLOG", "PHAS", );
}

TEST(AdlTest, bool)
{
    EXPECT_TRUE(OpenVisa::decode<bool>("ON"));
    EXPECT_TRUE(OpenVisa::decode<bool>("ON\n"));
    EXPECT_TRUE(OpenVisa::decode<bool>("ON\r\n"));
    EXPECT_FALSE(OpenVisa::decode<bool>("OFF"));
    EXPECT_FALSE(OpenVisa::decode<bool>("OFF\n"));
    EXPECT_FALSE(OpenVisa::decode<bool>("OFF\r\n"));
    EXPECT_TRUE(OpenVisa::decode<bool>("1"));
    EXPECT_TRUE(OpenVisa::decode<bool>("1\n"));
    EXPECT_TRUE(OpenVisa::decode<bool>("1\r\n"));
    EXPECT_TRUE(OpenVisa::decode<bool>("+1"));
    EXPECT_TRUE(OpenVisa::decode<bool>("+1\n"));
    EXPECT_TRUE(OpenVisa::decode<bool>("+1\r\n"));
    EXPECT_FALSE(OpenVisa::decode<bool>("0"));
    EXPECT_FALSE(OpenVisa::decode<bool>("0\n"));
    EXPECT_FALSE(OpenVisa::decode<bool>("0\r\n"));
    EXPECT_ANY_THROW(auto ret = OpenVisa::decode<bool>("NFF\n"));
}

TEST(AdlTest, double)
{
    EXPECT_EQ(0.1, OpenVisa::decode<double>("0.1\n"));
    EXPECT_EQ(0.1, OpenVisa::decode<double>("0.1\r\n"));
    EXPECT_EQ(-0.1, OpenVisa::decode<double>("-0.1\n"));
    EXPECT_EQ(-0.1, OpenVisa::decode<double>("-0.1\r\n"));
    EXPECT_EQ(0.1, OpenVisa::decode<double>("+0.1\n"));
    EXPECT_EQ(0.1, OpenVisa::decode<double>("+0.1\r\n"));
    EXPECT_EQ(100, OpenVisa::decode<double>("100A"));
    EXPECT_EQ(0.18, OpenVisa::decode<double>("1.8E-01"));
    EXPECT_EQ(0.18, OpenVisa::decode<double>("1.8e-01"));
    EXPECT_EQ(18, OpenVisa::decode<double>("1.8E+01"));
    EXPECT_EQ(18, OpenVisa::decode<double>("1.8e+01"));
}

TEST(AdlTest, int)
{
    EXPECT_EQ(1, OpenVisa::decode<int>("1\n"));
    EXPECT_EQ(1, OpenVisa::decode<int>("1\r\n"));
    EXPECT_EQ(-1, OpenVisa::decode<int>("-1\n"));
    EXPECT_EQ(-1, OpenVisa::decode<int>("-1\r\n"));
    EXPECT_EQ(1, OpenVisa::decode<int>("+1\n"));
    EXPECT_EQ(1, OpenVisa::decode<int>("+1\r\n"));
    EXPECT_EQ(100, OpenVisa::decode<int>("100.100"));
}

TEST(AdlTest, enum)
{
    EXPECT_EQ(EnumTest::MLinear, OpenVisa::decode<EnumTest>("MLIN\n"));
    EXPECT_EQ(EnumTest::MLogArithmic, OpenVisa::decode<EnumTest>("MLOG\n"));
    EXPECT_EQ(EnumTest::Phase, OpenVisa::decode<EnumTest>("PHAS\n"));
}

int main(int argc, char* argv[])
{
    std::cout << std::string("OpenVisa Version:") + OPENVISA_VERSION << std::endl;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}