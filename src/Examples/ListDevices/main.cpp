#include <OpenVisa/Object.h>

#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        OpenVisa::Object obj;
        for (auto usb : obj.listUSB())
        {
            std::cout << std::hex << "vid:" << usb.vendorId() << " pid:" << usb.productId() << " sn:" << usb.serialNumber() << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}