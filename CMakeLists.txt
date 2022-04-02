cmake_minimum_required(VERSION 3.20)
project(OpenVisa)

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/build)
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build/obj)
set(INSTALL_INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include)
if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(INSTALL_LIBRARYDIR ${CMAKE_INSTALL_PREFIX}/lib/x64)
    set(INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin/x64)
else()
    set(INSTALL_LIBRARYDIR ${CMAKE_INSTALL_PREFIX}/lib/x86)
    set(INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin/x86)
endif(${CMAKE_SIZEOF_VOID_P} EQUAL 8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DEBUG_POSTFIX "d")

if(MSVC)
  add_compile_options(/W3 /wd4251)
  if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.12.25835)
    set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
    set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")
  endif()
endif()

find_path(LIBUSB_INCLUDEDIR libusb.h DOC "Header directory for libusb." REQUIRED)
find_path(LIBUSB_LIBRARY libusb-1.0.* DOC "libusb library file." REQUIRED)
find_path(LIBUSB_BIN libusb-1.0.* DOC "libusb dll file." REQUIRED)

option(BUILD_OPENVISA_STATIC "Build OpenVisa static library." false)

add_subdirectory(src)
