#include <OpenVisa/AsciiArray.h>
#include <OpenVisa/BinaryArray.h>
#include <OpenVisa/Object.h>

#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
            return 0;
        auto test = std::stoi(argv[1]);
        switch (test)
        {
        case 0:
            {
                std::vector<int> values = OpenVisa::AsciiArray<int>(std::string(argv[3]), std::stoi(argv[2]));
                for (auto val : values)
                {
                    std::cout << val << " ";
                }
            }
            break;
        case 1:
            {
                std::vector<double> values = OpenVisa::AsciiArray<double>(std::string(argv[3]), std::stoi(argv[2]));
                for (auto val : values)
                {
                    std::cout << val << " ";
                }
            }
            break;
        case 2:
            {
                std::vector<int> values = OpenVisa::BinaryArray<int>(std::string("#18\x01\x00\x00\x00\x02\x00\x00\x00", 12));
                for (auto val : values)
                {
                    std::cout << val << " ";
                }
            }
            break;
        }
    }
    catch (const std::exception&)
    {
        std::cout << "exception";
    }
    return 0;
}