#include <OpenVisa/AsciiArray.h>
#include <OpenVisa/BinaryArray.h>
#include <OpenVisa/Object.h>
#include <gtest/gtest.h>

TEST(ArrayTest, AsciiArray_int)
{
    std::vector<int> vec1 { 123, 456, 789 };
    EXPECT_EQ(vec1, static_cast<std::vector<int>>(OpenVisa::AsciiArray<int>("123,456,789\n")));
    EXPECT_EQ(vec1, static_cast<std::vector<int>>(OpenVisa::AsciiArray<int>("abc,123,456,789\n", 4)));
    EXPECT_EQ(vec1, static_cast<std::vector<int>>(OpenVisa::AsciiArray<int>("123,456,789,\n")));
}

TEST(ArrayTest, AsciiArray_double)
{
    std::vector<double> vec1 { 123.1, 456.256, 789.993 };
    EXPECT_EQ(vec1, static_cast<std::vector<double>>(OpenVisa::AsciiArray<double>("123.1,456.256,789.993\n")));
    EXPECT_EQ(vec1, static_cast<std::vector<double>>(OpenVisa::AsciiArray<double>("abc,123.1,456.256,789.993\n", 4)));
    EXPECT_EQ(vec1, static_cast<std::vector<double>>(OpenVisa::AsciiArray<double>("123.1,456.256,789.993,\n")));
}

TEST(ArrayTest, AsciiArray_complex)
{
    std::vector<std::complex<double>> vec1 { { 123.1, 456.256 }, { 789.993, 646.444 } };
    EXPECT_EQ(
        vec1,
        static_cast<std::vector<std::complex<double>>>(OpenVisa::AsciiArray<std::complex<double>>("123.1,456.256,789.993,646.444\n")));
    EXPECT_EQ(vec1,
              static_cast<std::vector<std::complex<double>>>(
                  OpenVisa::AsciiArray<std::complex<double>>("abc,123.1,456.256,789.993,646.444\n", 4)));
    EXPECT_EQ(
        vec1,
        static_cast<std::vector<std::complex<double>>>(OpenVisa::AsciiArray<std::complex<double>>("123.1,456.256,789.993,646.444,\n")));
}

TEST(ArrayTest, BinaryArray_int)
{
    std::vector<int> vec1 { 1, 2 };
    EXPECT_EQ(vec1, static_cast<std::vector<int>>(OpenVisa::BinaryArray<int>(std::string("#18\x01\x00\x00\x00\x02\x00\x00\x00", 12))));
}

TEST(ArrayTest, BinaryArray_double)
{
    std::vector<double> vec1 { 1.0, 2.0 };
    std::string buffer(4 + sizeof(double) * vec1.size(), 0);
    buffer[0] = '#';
    buffer[1] = '2';
    buffer[2] = '1';
    buffer[3] = '6';
    memcpy(buffer.data() + 4, vec1.data(), vec1.size() * sizeof(double));
    EXPECT_EQ(vec1, static_cast<std::vector<double>>(OpenVisa::BinaryArray<double>(std::move(buffer))));
}

TEST(ArrayTest, BinaryArray_string)
{
    std::vector<double> vec1 { 1.0, 2.0 };
    std::string buffer(4 + sizeof(double) * vec1.size(), 0);
    buffer[0] = '#';
    buffer[1] = '2';
    buffer[2] = '1';
    buffer[3] = '6';
    std::string v { reinterpret_cast<const char*>(vec1.data()), vec1.size() * sizeof(std::vector<double>::value_type) };
    memcpy(buffer.data() + 4, vec1.data(), vec1.size() * sizeof(double));
    EXPECT_EQ(v, static_cast<std::string>(OpenVisa::BinaryArray<std::string>(std::move(buffer))));
}

TEST(ArrayTest, BinaryArray_wstring)
{
    std::vector<double> vec1 { 1.0, 2.0 };
    std::string buffer(4 + sizeof(double) * vec1.size(), 0);
    buffer[0] = '#';
    buffer[1] = '2';
    buffer[2] = '1';
    buffer[3] = '6';
    std::wstring v { reinterpret_cast<const wchar_t*>(vec1.data()),
                     vec1.size() * sizeof(std::vector<double>::value_type) / sizeof(wchar_t) };
    memcpy(buffer.data() + 4, vec1.data(), vec1.size() * sizeof(double));
    EXPECT_EQ(v, static_cast<std::wstring>(OpenVisa::BinaryArray<std::wstring>(std::move(buffer))));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}