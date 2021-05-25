set(TARGET_NAME "QCustomUiDesignerPlugin")

set(HEADERS
 "QCustomUiWidgets.h"
 "StageProgressBar.h"
 "IPAddressEdit.h"
 "MultiComboBox.h")
set(SOURCES 
 "QCustomUiWidgets.cpp"
 "StageProgressBar.cpp"
 "IPAddressEdit.cpp"
 "MultiComboBox.cpp"
)

file(GLOB RESOURCES *.qrc)
file(GLOB UI *.ui)

source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})
source_group("Resource" FILES ${RESOURCES})
source_group("UI" FILES ${UI})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Designer REQUIRED)

include_directories(../)

add_library(${TARGET_NAME} SHARED
    ${HEADERS}
    ${SOURCES}
    ${RESOURCES}
    ${UI}
)

target_link_libraries(${TARGET_NAME} PUBLIC QCustomUi PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Designer)
set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${QCUSTOMUI_VERSION} SOVERSION 1)

install(TARGETS ${TARGET_NAME}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${QTDIR}/plugins/designer)

install(FILES ${QCustomUi_BINARY_DIR}/src/QCustomUi/QCustomUi${CMAKE_SHARED_MODULE_SUFFIX}
        CONFIGURATIONS Release
        DESTINATION ${QTDIR}/bin)