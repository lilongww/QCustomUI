#include <OpenVisa/Object.h>
#include <gtest/gtest.h>

OpenVisa::Object object;

TEST(Connect, ConnectTest) { object.connect(OpenVisa::Address<OpenVisa::AddressType::USB>(0xF4EC, 0x1008, "SDS5XB0Q6R0186")); }

TEST(USBStress, SDS5034XStress)
{
    object.send("*RST");
    object.send("WAV:SOUR C1");
    auto p = object.query("WAV:MAXP?");
    object.send("WAV:POIN {}", p);
    for (int i = 0; i < 1000; ++i)
    {
        auto ret          = object.query("WAV:DATA?");
        auto bytes        = ret.size();
        auto protoSizeLen = ret[1] - '0';
        auto protoSize    = std::stoi(std::string(&ret[2], &ret[2] + protoSizeLen));
        EXPECT_EQ(protoSize, bytes - 2 - protoSizeLen - 2);
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}