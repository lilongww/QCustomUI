cmake_minimum_required(VERSION 3.20)
project(OpenVisa VERSION "0.12.0")
enable_testing()

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/build)
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build/obj)
set(INSTALL_INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include)
if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(INSTALL_LIBRARYDIR ${CMAKE_INSTALL_PREFIX}/lib/x64)
    set(INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin/x64)
    set(_platform "x64")
else()
    set(INSTALL_LIBRARYDIR ${CMAKE_INSTALL_PREFIX}/lib/x86)
    set(INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin/x86)
    set(_platform "Win32")
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

find_path(LIBUSB_INCLUDEDIR libusb.h
    PATHS "${CMAKE_CURRENT_LIST_DIR}/../libusb" "${CMAKE_CURRENT_LIST_DIR}/../../3rd/libusb"
    PATH_SUFFIXES libusb
    DOC "Header directory for libusb." REQUIRED)

if(EXISTS "${LIBUSB_INCLUDEDIR}/libusb.h")
    set(LIBUSB_INCLUDEDIR ${LIBUSB_INCLUDEDIR}/..)
endif()
message(${LIBUSB_INCLUDEDIR})
option(LIBUSB_USE_STATIC "Use static libusb library." false)

set(LIBUSB_LIBRARY_DEBUG_DIR)
set(LIBUSB_LIBRARY_RELEASE_DIR)
set(LIBUSB_RUNTIME_DEBUG_DIR)
set(LIBUSB_RUNTIME_RELEASE_DIR)

if(LIBUSB_USE_STATIC)
    find_library(LIBUSB_DEBUG_LIBRARY
        NAMES libusb libusb-1.0
        PATHS "${LIBUSB_INCLUDEDIR}/${_platform}/Debug/lib" "${LIBUSB_INCLUDEDIR}/build/v${MSVC_TOOLSET_VERSION}/${_platform}/Debug/lib" ${LIBUSB_LIBRARY_DEBUG_DIR} REQUIRED)
    find_library(LIBUSB_RELEASE_LIBRARY
        NAMES libusb libusb-1.0
        PATHS "${LIBUSB_INCLUDEDIR}/${_platform}/Release/lib" "${LIBUSB_INCLUDEDIR}/build/v${MSVC_TOOLSET_VERSION}/${_platform}/Release/lib" REQUIRED)
else(LIBUSB_USE_STATIC)
    find_library(LIBUSB_DEBUG_LIBRARY
        NAMES libusb libusb-1.0
        PATHS "${LIBUSB_INCLUDEDIR}/${_platform}/Debug/dll" "${LIBUSB_INCLUDEDIR}/build/v${MSVC_TOOLSET_VERSION}/${_platform}/Debug/dll" ${LIBUSB_LIBRARY_DEBUG_DIR} REQUIRED)
    find_library(LIBUSB_RELEASE_LIBRARY
        NAMES libusb libusb-1.0
        PATHS "${LIBUSB_INCLUDEDIR}/${_platform}/Release/dll" "${LIBUSB_INCLUDEDIR}/build/v${MSVC_TOOLSET_VERSION}/${_platform}/Release/dll" REQUIRED)
    if(WIN32)
        find_file(LIBUSB_DEBUG_DLL
            NAMES libusb.dll libusb-1.0.dll
            PATHS "${LIBUSB_INCLUDEDIR}/${_platform}/Debug/dll" "${LIBUSB_INCLUDEDIR}/build/v${MSVC_TOOLSET_VERSION}/${_platform}/Debug/dll" ${LIBUSB_LIBRARY_DEBUG_DIR} REQUIRED)
        find_file(LIBUSB_RELEASE_DLL
            NAMES libusb.dll libusb-1.0.dll
            PATHS "${LIBUSB_INCLUDEDIR}/${_platform}/Release/dll" "${LIBUSB_INCLUDEDIR}/build/v${MSVC_TOOLSET_VERSION}/${_platform}/Release/dll" ${LIBUSB_RUNTIME_RELEASE_DIR} REQUIRED)
    endif(WIN32)
endif(LIBUSB_USE_STATIC)
unset(_platform)

include(${PROJECT_SOURCE_DIR}/cmake/FindLibusb.cmake)

openvisa_find_libusb(${LIBUSB_DEBUG_LIBRARY} ${LIBUSB_RELEASE_LIBRARY} ${LIBUSB_DEBUG_DLL} ${LIBUSB_RELEASE_DLL} ${LIBUSB_USE_STATIC})

option(BUILD_OPENVISA_STATIC "Build OpenVisa static library." false)
option(BUILD_TEST "Build test applications." true)

include(${PROJECT_SOURCE_DIR}/cmake/ExportHelper.cmake)
export_config(${PROJECT_NAME} ${INSTALL_LIBRARYDIR} ${INSTALL_INCLUDEDIR} ${LIBUSB_DEBUG_LIBRARY} ${LIBUSB_RELEASE_LIBRARY} ${LIBUSB_DEBUG_DLL} ${LIBUSB_RELEASE_DLL} ${LIBUSB_USE_STATIC})
install(FILES ${PROJECT_SOURCE_DIR}/cmake/FindLibusb.cmake DESTINATION ${INSTALL_LIBRARYDIR}/cmake)

add_subdirectory(src)
