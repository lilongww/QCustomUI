string( REGEX REPLACE ".*/(.*)" "\\1" TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} ) 

include_directories(../../)

file(GLOB HEADERS *.h)
file(GLOB SOURCES *.cpp)
file(GLOB RESOURCES *.qrc)
file(GLOB UI *.ui)

source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})
source_group("Resource" FILES ${RESOURCES})
source_group("UI" FILES ${UI})

add_executable(${TARGET_NAME} WIN32
 ${HEADERS}
 ${SOURCES}
 ${RESOURCES}
 ${UI}
)

set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "Examples")
target_link_libraries(${TARGET_NAME} PUBLIC QCustomUi PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets)

install(TARGETS ${TARGET_NAME}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${INSTALL_BINDIR}/Debug)
install(TARGETS ${TARGET_NAME}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${INSTALL_BINDIR}/Release)
