﻿set(TARGET_NAME "QCustomUiDesignerPlugin")

file(GLOB HEADERS "*.h")
file(GLOB SOURCES "*.cpp")
file(GLOB RESOURCES *.qrc)
file(GLOB UI *.ui)

source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})
source_group("Resource" FILES ${RESOURCES})
source_group("UI" FILES ${UI})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets UiPlugin REQUIRED)
file(REAL_PATH "${QT_DIR}/../../.." QTDIR)

include_directories(../)

add_library(${TARGET_NAME} SHARED
    ${HEADERS}
    ${SOURCES}
    ${RESOURCES}
    ${UI}
     )

target_link_libraries(${TARGET_NAME} PUBLIC QCustomUi PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::UiPlugin)

cmake_path(GET QTDIR PARENT_PATH QT_ROOT)
cmake_path(GET QT_ROOT PARENT_PATH QT_ROOT)

install(TARGETS ${TARGET_NAME}
        CONFIGURATIONS Release
        RUNTIME DESTINATION "${QTDIR}/plugins/designer")
install(TARGETS ${TARGET_NAME}
        CONFIGURATIONS Release
        RUNTIME DESTINATION "${QT_ROOT}/Tools/QtCreator/bin/plugins/designer")
install(FILES ${QCustomUi_BINARY_DIR}/src/QCustomUi/QCustomUi${CMAKE_SHARED_MODULE_SUFFIX}
        CONFIGURATIONS Release
        DESTINATION "${QTDIR}/bin")
install(FILES ${QCustomUi_BINARY_DIR}/src/QCustomUi/QCustomUi${CMAKE_SHARED_MODULE_SUFFIX}
        CONFIGURATIONS Release
        DESTINATION "${QT_ROOT}/Tools/QtCreator/bin")
