set(TARGET_NAME "QCustomUiDesignerPlugin")

set(HEADERS
 "QCustomUiWidgets.h"
 "StageProgressBar.h"
 "IPAddressEdit.h"
 "MultiComboBox.h"
 "ComboBox.h"
 "TableView.h"
 "ClassifyTreeView.h"
 "EditLabel.h"
 "SwitchButton.h"
 "CircleProgressBar.h")
set(SOURCES 
 "QCustomUiWidgets.cpp"
 "StageProgressBar.cpp"
 "IPAddressEdit.cpp"
 "MultiComboBox.cpp"
 "ComboBox.cpp"
 "TableView.cpp"
 "ClassifyTreeView.cpp"
 "EditLabel.cpp"
 "SwitchButton.cpp"
 "CircleProgressBar.cpp"
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

find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets UiPlugin REQUIRED)

include_directories(../)

add_library(${TARGET_NAME} SHARED
    ${HEADERS}
    ${SOURCES}
    ${RESOURCES}
    ${UI}
     )

target_link_libraries(${TARGET_NAME} PUBLIC QCustomUi PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::UiPlugin)
set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${QCUSTOMUI_VERSION} SOVERSION 1)

install(TARGETS ${TARGET_NAME}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${QTDIR}/plugins/designer)
install(TARGETS ${TARGET_NAME}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${QTDIR}/../../Tools/QtCreator/bin/plugins/designer)

install(FILES ${QCustomUi_BINARY_DIR}/src/QCustomUi/QCustomUi${CMAKE_SHARED_MODULE_SUFFIX}
        CONFIGURATIONS Release
        DESTINATION ${QTDIR}/bin)
install(FILES ${QCustomUi_BINARY_DIR}/src/QCustomUi/QCustomUi${CMAKE_SHARED_MODULE_SUFFIX}
        CONFIGURATIONS Release
        DESTINATION ${QTDIR}/../../Tools/QtCreator/bin)
