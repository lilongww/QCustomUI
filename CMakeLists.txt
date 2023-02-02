cmake_minimum_required(VERSION 3.20)

project(QCustomUi VERSION "1.10.0")
enable_testing()

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

include(${CMAKE_CURRENT_LIST_DIR}/cmake/Common.cmake)
if (MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall)
endif()

include(${PROJECT_SOURCE_DIR}/cmake/ExportHelper.cmake)

export_config(${PROJECT_NAME} ${INSTALL_LIBRARYDIR} ${INSTALL_INCLUDEDIR})

add_subdirectory(src)
