#include <OpenVisa/Object.h>

#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        OpenVisa::Object obj;
        // obj.connect(OpenVisa::Address<OpenVisa::AddressType::HiSLIP>("192.168.2.111"));
        //  obj.connect(OpenVisa::Address<OpenVisa::AddressType::VXI11>("192.168.2.111", "inst0"));
        obj.connect(OpenVisa::Address<OpenVisa::AddressType::USB>(0xF4EC, 0x1102, "SDG2XFBQ6R2965"));
        for (int i = 0; i < 1024; i++)
            std::cout << obj.query("*IDN?");
        // auto buffer = obj.query("MMEMory:DATA? 'C:\\R_S\\Instr\\install\\FoxitReader_Setup.exe'");
        // auto buffer = obj.query("MMEMory:DATA? 'C:\\Program Files (x86)\\Rohde-Schwarz\\RsVisa\\VisaManual.pdf'");
        // std::cout << buffer.size();
        // std::string buffer(1024 * 1024, '1');
        // auto str = std::to_string(1024 * 1024);
        // obj.send("MMEM:CDIR 'C:\\R_S'");
        // obj.send("MMEMory:DATA '1.txt',#{}{}{}", str.size(), str, buffer);
        // std::cout << obj.query("*IDN?");
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}