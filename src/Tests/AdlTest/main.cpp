#include <OpenVisa/Object.h>
#include <OpenVisa/Utils.h>

#include <iostream>

enum class EnumTest
{
    MLinear,
    MLogArithmic,
    Phase,
    UPhase,
    ImagInary,
    Real,
    Polar,
    Smith,
    SAdmittance,
    Swr,
    GDelay,
    Kelvin,
    Fahrenheit,
    Celsius,
    PPhase
};

namespace OpenVisa
{
VISA_ENUM_ADL_DECLARE(EnumTest::MLinear,
                      EnumTest::PPhase,
                      "MLIN",
                      "MLOG",
                      "PHAS",
                      "UPH",
                      "IMAG",
                      "REAL",
                      "POL",
                      "SMIT",
                      "SADM",
                      "SWR",
                      "GDEL",
                      "KELV",
                      "FAHR",
                      "CELS",
                      "PPH");
}
int main(int argc, char* argv[])
{
    try
    {
        if (argc < 3)
            return 0;
        auto test = std::stoi(argv[1]);
        switch (test)
        {
        case 0:
            std::cout << std::boolalpha << OpenVisa::decode<bool>(argv[2]);
            break;
        case 1:
            std::cout << OpenVisa::decode<double>(argv[2]);
            break;
        case 2:
            std::cout << OpenVisa::decode<int>(argv[2]);
            break;
        case 3:
            std::cout << static_cast<int>(OpenVisa::decode<EnumTest>(argv[2]));
            break;
        }
    }
    catch (const std::exception&)
    {
        std::cout << "exception";
    }
    return 0;
}