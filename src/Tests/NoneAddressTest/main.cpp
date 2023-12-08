#include <OpenVisa/Object.h>
#include <gtest/gtest.h>

TEST(AdlTest, bool)
{
    OpenVisa::Object object;
    EXPECT_THROW(object.connect(OpenVisa::Address<OpenVisa::AddressType::RawSocket>("", 0)), std::exception);
}
int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}