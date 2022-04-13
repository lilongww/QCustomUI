#include <OpenVisa/Object.h>

#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        OpenVisa::Object obj;
        // obj.connect(OpenVisa::Address<OpenVisa::AddressType::VXI11>("192.168.2.111"));
        obj.connect(OpenVisa::Address<OpenVisa::AddressType::HiSLIP>("192.168.2.111", "hislip0"));
        std::cout << obj.query("*IDN?");
        // auto buffer = obj.query("MMEMory:DATA? 'C:\\R_S\\Instr\\install\\FoxitReader_Setup.exe'");
        // std::cout << buffer.size();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}