set(TARGET_NAME OpenVisa)

find_package(Boost COMPONENTS regex system REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/lib/include ${Boost_INCLUDE_DIRS} ${LIBUSB_INCLUDEDIR})

file(GLOB HEADERS "*.h")
file(GLOB PRIVATES "Private/*.h" "Private/*.cpp")
file(GLOB SOURCES "*.cpp")
file(GLOB DOCS "Docs/*.qdoc")
source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})
source_group("Privates" FILES ${PRIVATES})
source_group("Docs" FILES ${DOCS})

add_library(${TARGET_NAME}
    ${HEADERS}
    ${SOURCES}
    ${PRIVATES}
    ${DOCS}
    )

set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "Drivers")

install(TARGETS ${TARGET_NAME}
    CONFIGURATIONS Debug
    ARCHIVE DESTINATION ${INSTALL_LIBRARYDIR}
    PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDEDIR}/${TARGET_NAME}
)
install(TARGETS ${TARGET_NAME}
    CONFIGURATIONS Release
    ARCHIVE DESTINATION ${INSTALL_LIBRARYDIR}
    PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDEDIR}/${TARGET_NAME}
)
