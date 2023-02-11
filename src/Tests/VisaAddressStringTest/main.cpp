#include <OpenVisa/Address.h>
#include <OpenVisa/Object.h>
#include <gtest/gtest.h>

using namespace OpenVisa;

TEST(VisaAddressStringTest, RawSocket)
{
    EXPECT_EQ(Object::toVisaAddressString(Address<AddressType::RawSocket>("192.168.1.1", 5025)), "TCPIP::192.168.1.1::5025::SOCKET");
}

TEST(VisaAddressStringTest, USB)
{
    EXPECT_EQ(Object::toVisaAddressString(Address<AddressType::USB>(0xFFFF, 0x6300, "abcdefghijklmn")),
              "USB::0XFFFF::0X6300::abcdefghijklmn::INSTR");
}

TEST(VisaAddressStringTest, VXI11)
{
    EXPECT_EQ(Object::toVisaAddressString(Address<AddressType::VXI11>("192.168.1.1")), "TCPIP::192.168.1.1::inst0::INSTR");
}

TEST(VisaAddressStringTest, HiSLIP)
{
    EXPECT_EQ(Object::toVisaAddressString(Address<AddressType::HiSLIP>("192.168.1.1")), "TCPIP::192.168.1.1::hislip0");
}

TEST(VisaAddressStringTest, SerialPort)
{
    EXPECT_EQ(Object::toVisaAddressString(Address<AddressType::SerialPort>("COM3")), "ASRL3::INSTR");
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
