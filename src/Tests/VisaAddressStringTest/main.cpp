#include <OpenVisa/Address.h>
#include <OpenVisa/Object.h>
#include <gtest/gtest.h>

using namespace OpenVisa;

TEST(VisaAddressStringTest, RawSocket)
{
    auto addr = Address<AddressType::RawSocket>("192.168.1.1", 5025);
    auto str  = "TCPIP::192.168.1.1::5025::SOCKET";
    EXPECT_EQ(Object::toVisaAddressString(addr), str);
    EXPECT_EQ(std::get<1>(Object::fromVisaAddressString(str)), addr);
    EXPECT_EQ(std::get<0>(Object::fromVisaAddressString("TCPIP::192.168.1.1::SOCKET")), std::monostate {});
}

TEST(VisaAddressStringTest, SerialPort)
{
    auto addr = Address<AddressType::SerialPort>("COM3");
    auto str  = "ASRL3::INSTR";
    EXPECT_EQ(Object::toVisaAddressString(addr), str);
    EXPECT_EQ(std::get<2>(Object::fromVisaAddressString(str)).portName(), addr.portName());
}

TEST(VisaAddressStringTest, USB)
{
    auto addr = Address<AddressType::USB>(0xFFFF, 0x6300, "abcdefghijklmn");
    auto str  = "USB::0XFFFF::0X6300::abcdefghijklmn::INSTR";
    EXPECT_EQ(Object::toVisaAddressString(addr), str);
    EXPECT_EQ(std::get<3>(Object::fromVisaAddressString(str)), addr);
}

TEST(VisaAddressStringTest, VXI11)
{
    auto addr = Address<AddressType::VXI11>("192.168.1.1");
    auto str  = "TCPIP::192.168.1.1::inst0::INSTR";
    EXPECT_EQ(Object::toVisaAddressString(addr), str);
    EXPECT_EQ(std::get<4>(Object::fromVisaAddressString(str)), addr);
}

TEST(VisaAddressStringTest, HiSLIP)
{
    auto addr = Address<AddressType::HiSLIP>("192.168.1.1");
    auto str  = "TCPIP::192.168.1.1::hislip0";
    EXPECT_EQ(Object::toVisaAddressString(addr), str);
    EXPECT_EQ(std::get<5>(Object::fromVisaAddressString(str)), addr);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
