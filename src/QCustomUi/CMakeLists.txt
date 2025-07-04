﻿set(TARGET_NAME QCustomUi)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

message("Find or download pegtl...")
include(FetchContent)
FetchContent_Declare(
  pegtl
  #GIT_REPOSITORY https://github.com/taocpp/PEGTL.git #github 原仓库地址
  GIT_REPOSITORY https://gitee.com/mirrors/pegtl.git #gitee镜像地址
  GIT_TAG        main
  FIND_PACKAGE_ARGS NAMES pegtl
)
set(PEGTL_BUILD_TESTS OFF)
set(PEGTL_BUILD_EXAMPLES OFF)
FetchContent_MakeAvailable(pegtl)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
if(${QT_VERSION_MAJOR} EQUAL 5)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network WinExtras SerialPort REQUIRED)
else(${QT_VERSION_MAJOR} EQUAL 5)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network SerialPort REQUIRED)
endif(${QT_VERSION_MAJOR} EQUAL 5)
file(REAL_PATH "${QT_DIR}/../../.." QTDIR)
message("-- Found Qt ${QT_VERSION}")
message("-- QTDIR: ${QTDIR}")
include_directories("./")

option(FRAMELESS_PURE_QT_BACKEND "Frameless window pure Qt backend" FALSE)

if(${FRAMELESS_PURE_QT_BACKEND})
    add_compile_definitions(QCUSTOMUI_LIB UNICODE _WIN32_WINNT=0x0600 QCUSTOMUI_FRAMELESS_USE_PURE_QT)
else()
    add_compile_definitions(QCUSTOMUI_LIB UNICODE _WIN32_WINNT=0x0600)
endif()

file(GLOB PRIVATRE_HEADERS ./Private/*.h)
file(GLOB PRIVATRE_SOURCES ./Private/*.cpp)
file(GLOB UI ./*.ui)
file(GLOB DOCS ./Docs/*.qdoc)

set(RESOURCES
 "${PROJECT_SOURCE_DIR}/README.md"
 "QCustomUi.qrc"
 "Resources/stylesheet/sw-light.css")

set(HEADERS
 "qcustomui_global.h"
)

set(SOURCES )

set(LOG_HEADERS
 "QCtmLogEvent.h"
 "QCtmLogManager.h"
 "QCtmLogData.h"
 "QCtmLogModel.h"
 "QCtmLogWidget.h"
 "QCtmAbstractLogModel.h"
)

set(LOG_SOURCES
 "QCtmLogWidget.cpp"
 "QCtmLogEvent.cpp"
 "QCtmLogManager.cpp"
 "QCtmLogModel.cpp"
 "QCtmLogData.cpp"
 "QCtmAbstractLogModel.cpp"
)

set(INPUT_HEADERS
 "QCtmIPAddressEdit.h"
 "QCtmMultiComboBox.h"
 "QCtmComboBox.h"
 "QCtmInputDialog.h"
 "QCtmPathBrowser.h"
 "QCtmDigitKeyboard.h"
 "QCtmLongLongSpinBox.h"
 "QCtmULongLongSpinBox.h"
 "QCtmHexLineEdit.h"
 "QCtmSerialPortComboBox.h"
 "QCtmRangeSlider.h"
)

set(INPUT_SOURCES
 "QCtmIPAddressEdit.cpp"
 "QCtmMultiComboBox.cpp"
 "QCtmComboBox.cpp"
 "QCtmInputDialog.cpp"
 "QCtmPathBrowser.cpp"
 "QCtmDigitKeyboard.cpp"
 "QCtmLongLongSpinBox.cpp"
 "QCtmULongLongSpinBox.cpp"
 "QCtmHexLineEdit.cpp"
 "QCtmSerialPortComboBox.cpp"
 "QCtmRangeSlider.cpp"
)

set(CONTAINERS_HEADERS
 "QCtmDialog.h"
 "QCtmWindow.h"
 "QCtmDrawerWidget.h"
 "QCtmDrawerItemWidget.h"
 "QCtmToolBox.h"
 "QCtmTabPage.h"
 "QCtmTabWidget.h"
 "QCtmNavigationSidePane.h"
 "QCtmNavigationSideBar.h"
)

set(CONTAINERS_SOURCES
 "QCtmDialog.cpp"
 "QCtmWindow.cpp"
 "QCtmDrawerWidget.cpp"
 "QCtmDrawerItemWidget.cpp"
 "QCtmToolBox.cpp"
 "QCtmTabPage.cpp"
 "QCtmTabWidget.cpp"
 "QCtmNavigationSidePane.cpp"
 "QCtmNavigationSideBar.cpp"
)

set(MESSAGETIP_HEADERS
 "QCtmMessageTipData.h"
 "QCtmMessageTipModel.h"
 "QCtmMessageTipButton.h"
 "QCtmMessageTipView.h"
 "QCtmAbstractMessageTipModel.h"
 "QCtmAbstractMessageTipView.h"
)

set(MESSAGETIP_SOURCES
 "QCtmMessageTipView.cpp"
 "QCtmMessageTipData.cpp"
 "QCtmMessageTipModel.cpp"
 "QCtmMessageTipButton.cpp"
 "QCtmAbstractMessageTipModel.cpp"
 "QCtmAbstractMessageTipView.cpp"
)

set(DISPLAYWIDGETS_HEADERS
 "QCtmEditLabel.h"
 "QCtmMessageBox.h"
 "QCtmStageProgressBar.h"
 "QCtmLoadingDialog.h"
 "QCtmNavigationBar.h"
 "QCtmTitleBar.h"
 "QCtmCircleProgressBar.h"
)
set(DISPLAYWIDGETS_SOURCES
 "QCtmEditLabel.cpp"
 "QCtmMessageBox.cpp"
 "QCtmStageProgressBar.cpp"
 "QCtmLoadingDialog.cpp"
 "QCtmNavigationBar.cpp"
 "QCtmTitleBar.cpp"
 "QCtmCircleProgressBar.cpp"
)

set(VIEW_HEADERS
 "QCtmTableItemDelegate.h"
 "QCtmTableView.h"
 "QCtmClassifyTreeView.h"
 "QCtmClassifyTreeModel.h"
 "QCtmClassifyTreeItem.h"
 "QCtmHeaderView.h"
 "QCtmAbstractMultiPageItemModel.h"
 "QCtmAbstractMultiPageTableModel.h"
 "QCtmMultiPageStringListModel.h"
 "QCtmMultiPageButtonBox.h"
 "QCtmRecentModel.h"
 "QCtmRecentView.h"
 "QCtmRecentViewDelegate.h"
 "QCtmTableViewButtonsDelegate.h"
)
set(VIEW_SOURCES
 "QCtmTableItemDelegate.cpp"
 "QCtmTableView.cpp"
 "QCtmClassifyTreeView.cpp"
 "QCtmClassifyTreeModel.cpp"
 "QCtmClassifyTreeItem.cpp"
 "QCtmHeaderView.cpp"
 "QCtmAbstractMultiPageItemModel.cpp"
 "QCtmAbstractMultiPageTableModel.cpp"
 "QCtmMultiPageStringListModel.cpp"
 "QCtmMultiPageButtonBox.cpp"
 "QCtmRecentModel.cpp"
 "QCtmRecentView.cpp"
 "QCtmRecentViewDelegate.cpp"
 "QCtmTableViewButtonsDelegate.cpp"
)

set(TOOLS_HEADERS
 "QCtmStyleSheet.h"
 "QCtmColor.h"
 "QCtmApplication.h"
 "QCtmFile.h"
 "QCtmSignalBlocker.h"
)

set(TOOLS_SOURCES
 "QCtmColor.cpp"
 "QCtmStyleSheet.cpp"
 "QCtmApplication.cpp"
 "QCtmFile.cpp"
)

set(BUTTONS_HEADERS "QCtmSwitchButton.h" "QCtmSelectionButtonBox.h")

set(BUTTONS_SOURCES "QCtmSwitchButton.cpp" "QCtmSelectionButtonBox.cpp")

set(UI_PRIVATE
 "Private/QCtmToolBoxItem.ui")

if(MSVC)
    set(QCUSTOMUI_VERSIONINFO_RC ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.rc)
    configure_file("${PROJECT_SOURCE_DIR}/cmake/${TARGET_NAME}.rc.in" "${QCUSTOMUI_VERSIONINFO_RC}")
    list(APPEND RESOURCES ${QCUSTOMUI_VERSIONINFO_RC})
endif(MSVC)

set(QCUSTOMUI_VERSION_H ${CMAKE_CURRENT_BINARY_DIR}/Version.h)
configure_file("${PROJECT_SOURCE_DIR}/cmake/Version.h.in" "${QCUSTOMUI_VERSION_H}")
list(APPEND HEADERS ${QCUSTOMUI_VERSION_H}) 

source_group("Headers" FILES ${HEADERS})
source_group("Headers/Log" FILES ${LOG_HEADERS})
source_group("Headers/Private" FILES ${PRIVATRE_HEADERS})
source_group("Headers/Input" FILES ${INPUT_HEADERS})
source_group("Headers/Containers" FILES ${CONTAINERS_HEADERS})
source_group("Headers/MessageTip" FILES ${MESSAGETIP_HEADERS})
source_group("Headers/Display Widgets" FILES ${DISPLAYWIDGETS_HEADERS})
source_group("Headers/Views" FILES ${VIEW_HEADERS})
source_group("Headers/Tools" FILES ${TOOLS_HEADERS})
source_group("Headers/Buttons" FILES ${BUTTONS_HEADERS})

source_group("Sources" FILES ${SOURCES})
source_group("Sources/Log" FILES ${LOG_SOURCES})
source_group("Sources/Private" FILES ${PRIVATRE_SOURCES})
source_group("Sources/Input" FILES ${INPUT_SOURCES})
source_group("Sources/Containers" FILES ${CONTAINERS_SOURCES})
source_group("Sources/MessageTip" FILES ${MESSAGETIP_SOURCES})
source_group("Sources/Display Widgets" FILES ${DISPLAYWIDGETS_SOURCES})
source_group("Sources/Views" FILES ${VIEW_SOURCES})
source_group("Sources/Tools" FILES ${TOOLS_SOURCES})
source_group("Sources/Buttons" FILES ${BUTTONS_SOURCES})

source_group("UI" FILES ${UI})
source_group("UI/Private" FILES ${UI_PRIVATE})
source_group("Resource" FILES ${RESOURCES})

if(NOT ${BUILD_STATIC})
    set(_QCUSTOMUI_OPTION SHARED)
else()
    add_compile_definitions(-DBUILD_STATIC)
endif()

add_library(${TARGET_NAME} ${_QCUSTOMUI_OPTION}
 ${HEADERS}
 ${PRIVATRE_HEADERS}
 ${LOG_HEADERS}
 ${INPUT_HEADERS}
 ${CONTAINERS_HEADERS}
 ${MESSAGETIP_HEADERS}
 ${DISPLAYWIDGETS_HEADERS}
 ${VIEW_HEADERS}
 ${TOOLS_HEADERS}
 ${BUTTONS_HEADERS}

 ${SOURCES}
 ${PRIVATRE_SOURCES}
 ${LOG_SOURCES}
 ${CONTAINERS_SOURCES}
 ${INPUT_SOURCES}
 ${MESSAGETIP_SOURCES}
 ${DISPLAYWIDGETS_SOURCES}
 ${VIEW_SOURCES}
 ${TOOLS_SOURCES}
 ${BUTTONS_SOURCES}

 ${UI}
 ${UI_PRIVATE}
 ${RESOURCES}
 ${DOCS}
)

if(WIN32)
    if(${QT_VERSION_MAJOR} EQUAL 5)
        target_link_libraries(${TARGET_NAME} PUBLIC dwmapi PRIVATE Qt::Widgets Qt::Network Qt::WinExtras Qt::WidgetsPrivate Qt::SerialPort taocpp::pegtl)
    else(${QT_VERSION_MAJOR} EQUAL 5)
        target_link_libraries(${TARGET_NAME} PUBLIC dwmapi PRIVATE Qt::Widgets Qt::Network Qt::WidgetsPrivate Qt::SerialPort taocpp::pegtl)
    endif(${QT_VERSION_MAJOR} EQUAL 5)
else ()
    target_link_libraries(${TARGET_NAME} PRIVATE Qt::Widgets Qt::Network Qt::WidgetsPrivate Qt::SerialPort taocpp::pegtl)
endif(WIN32)
set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)

file(GLOB PUBLIC_HEADERS ./*.h)
list(APPEND PUBLIC_HEADERS ${QCUSTOMUI_VERSION_H})
set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

export_install_target(${TARGET_NAME} ${INSTALL_INCLUDEDIR} ${INSTALL_LIBRARYDIR} ${INSTALL_BINDIR})