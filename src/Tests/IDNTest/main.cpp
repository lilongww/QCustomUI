#include <OpenVisa/IDN.h>
#include <gtest/gtest.h>

constexpr auto FullIDN  = "Siglent Technologies,SDS5104X,SDS5XDAD2R0160,4.6.0.8.7R1";
constexpr auto EmptyIDN = "Siglent Technologies,SDS5104X,,0";

TEST(AdlTest, bool)
{
    {
        OpenVisa::IDN idn(FullIDN);
        EXPECT_EQ(idn.manufacturer(), "Siglent Technologies");
        EXPECT_EQ(idn.model(), "SDS5104X");
        EXPECT_EQ(idn.serialNumber(), "SDS5XDAD2R0160");
        EXPECT_EQ(idn.firmwareRevision(), "4.6.0.8.7R1");
    }
    {
        OpenVisa::IDN idn(EmptyIDN);
        EXPECT_EQ(idn.manufacturer(), "Siglent Technologies");
        EXPECT_EQ(idn.model(), "SDS5104X");
        EXPECT_TRUE(idn.serialNumber().empty());
        EXPECT_EQ(idn.firmwareRevision(), "0");
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}