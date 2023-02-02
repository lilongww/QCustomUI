set(TARGET_NAME OpenVisa)

add_compile_definitions(OPENVISA_LIB)
link_directories(${LIBUSB_LIBRARY})
if(BUILD_OPENVISA_STATIC)
    add_compile_definitions(BUILD_OPENVISA_STATIC)
    set(OPENVISA_OPT)
else()
    set(OPENVISA_OPT SHARED)
endif()

find_package(Boost COMPONENTS regex system REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/lib/include ${Boost_INCLUDE_DIRS} ${LIBUSB_INCLUDEDIR})

file(GLOB HEADERS "*.h")
file(GLOB PRIVATES "Private/*.h" "Private/*.cpp")
file(GLOB SOURCES "*.cpp")
file(GLOB DOCS "Docs/*.qdoc")
file(GLOB RESOURCES "*.rc")

if(MSVC)
    set(OPENVISA_VERSIONINFO_RC ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.rc)
    configure_file("${PROJECT_SOURCE_DIR}/cmake/${TARGET_NAME}.rc.in" "${OPENVISA_VERSIONINFO_RC}")
    list(APPEND RESOURCES ${OPENVISA_VERSIONINFO_RC}) 
endif(MSVC)

source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})
source_group("Privates" FILES ${PRIVATES})
source_group("Docs" FILES ${DOCS})
source_group("Resources" FILES ${RESOURCES})

add_library(${TARGET_NAME} ${OPENVISA_OPT}
    ${HEADERS}
    ${SOURCES}
    ${PRIVATES}
    ${DOCS}
    ${RESOURCES}
    )

if(NOT BUILD_OPENVISA_STATIC)
    if(WIN32)
        target_link_libraries(${TARGET_NAME} PUBLIC ${LIBUSB_LIBRARY} Setupapi)
    else()
        target_link_libraries(${TARGET_NAME} PUBLIC ${LIBUSB_LIBRARY} Setupapi)
    endif()
endif()
set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

export_install_target(${TARGET_NAME} ${INSTALL_INCLUDEDIR} ${INSTALL_LIBRARYDIR} ${INSTALL_BINDIR})