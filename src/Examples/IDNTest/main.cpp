#include <OpenVisa/Object.h>

#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        OpenVisa::Object obj;
        //obj.connect(OpenVisa::Address<OpenVisa::AddressType::VXI11>("192.168.2.111"));
        obj.connect(OpenVisa::Address<OpenVisa::AddressType::HiSLIP>("192.168.2.111"));
        for (int i = 0; i < 1024; i++)
        {
            std::cout << obj.query("*IDN?") << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}