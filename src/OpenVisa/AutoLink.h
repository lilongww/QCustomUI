#pragma once

#ifndef OPEN_VISA_LIB
#ifdef _MSC_VER
#pragma comment(lib, "libusb-1.0.lib")
#ifdef _DEBUG
#pragma comment(lib, "OpenVisad.lib")
#else
#pragma comment(lib, "OpenVisa.lib")
#endif
#endif
#endif
#ifdef WIN32
#pragma comment(lib, "Setupapi.lib")
#endif
