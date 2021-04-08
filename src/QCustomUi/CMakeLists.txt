set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets WinExtras Network REQUIRED)

include_directories("./")

add_compile_definitions(QCUSTOMUI_LIB UNICODE)

file(GLOB PRIVATRE_HEADERS ./Private/*.h)
file(GLOB PRIVATRE_SOURCES ./Private/*.cpp)
file(GLOB UI ./*.ui)
file(GLOB RESOURCES ./*.qrc)
file(GLOB DOCS ./Docs/*.qdoc)
list(APPEND RESOURCES "${PROJECT_SOURCE_DIR}/README.md")

set(HEADERS
 "qcustomui_global.h"
 "resource.h"
)

set(SOURCES )

set(LOG_HEADERS
 "QCtmLogEvent.h"
 "QCtmLogManager.h"
 "QCtmLogData.h"
 "QCtmLogFilterModel.h"
 "QCtmLogModel.h"
 "QCtmLogWidget.h"
 "QCtmAbstractLogModel.h"
)

set(LOG_SOURCES
 "QCtmLogWidget.cpp"
 "QCtmLogEvent.cpp"
 "QCtmLogFilterModel.cpp"
 "QCtmLogManager.cpp"
 "QCtmLogModel.cpp"
 "QCtmLogData.cpp"
 "QCtmAbstractLogModel.cpp"
)

set(INPUT_HEADERS
 "QCtmIPAddressEdit.h"
 "QCtmMultiComboBox.h"
 "QCtmComboBox.h"
)

set(INPUT_SOURCES
 "QCtmIPAddressEdit.cpp"
 "QCtmMultiComboBox.cpp"
 "QCtmComboBox.cpp"
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
)
set(DISPLAYWIDGETS_SOURCES
 "QCtmEditLabel.cpp"
 "QCtmMessageBox.cpp"
 "QCtmStageProgressBar.cpp"
 "QCtmLoadingDialog.cpp"
 "QCtmNavigationBar.cpp"
 "QCtmTitleBar.cpp"

)

set(VIEW_HEADERS
 "QCtmTableItemDelegate.h"
 "QCtmTableView.h"
 "QCtmClassifyTreeView.h"
 "QCtmClassifyTreeModel.h"
 "QCtmClassifyTreeItem.h"
 "QCtmHeaderView.h"
)
set(VIEW_SOURCES
 "QCtmTableItemDelegate.cpp"
 "QCtmTableView.cpp"
 "QCtmClassifyTreeView.cpp"
 "QCtmClassifyTreeModel.cpp"
 "QCtmClassifyTreeItem.cpp"
 "QCtmHeaderView.cpp"
)

set(TOOLS_HEADERS
 "QCtmStyleSheet.h"
 "QCtmColor.h"
 "QCtmApplication.h"
)

set(TOOLS_SOURCES
 "QCtmColor.cpp"
 "QCtmStyleSheet.cpp"
 "QCtmApplication.cpp"
)

if(WIN32)
 list(APPEND RESOURCES "QCustomUi.rc")
endif()

source_group("Headers" FILES ${HEADERS})
source_group("Headers/Log" FILES ${LOG_HEADERS})
source_group("Headers/Private" FILES ${PRIVATRE_HEADERS})
source_group("Headers/Input" FILES ${INPUT_HEADERS})
source_group("Headers/Containers" FILES ${CONTAINERS_HEADERS})
source_group("Headers/MessageTip" FILES ${MESSAGETIP_HEADERS})
source_group("Headers/Display Widgets" FILES ${DISPLAYWIDGETS_HEADERS})
source_group("Headers/Views" FILES ${VIEW_HEADERS})
source_group("Headers/Tools" FILES ${TOOLS_HEADERS})

source_group("Sources" FILES ${SOURCES})
source_group("Sources/Log" FILES ${LOG_SOURCES})
source_group("Sources/Private" FILES ${PRIVATRE_SOURCES})
source_group("Sources/Input" FILES ${INPUT_SOURCES})
source_group("Sources/Containers" FILES ${CONTAINERS_SOURCES})
source_group("Sources/MessageTip" FILES ${MESSAGETIP_SOURCES})
source_group("Sources/Display Widgets" FILES ${DISPLAYWIDGETS_SOURCES})
source_group("Sources/Views" FILES ${VIEW_SOURCES})
source_group("Sources/Tools" FILES ${TOOLS_SOURCES})

source_group("UI" FILES ${UI})
source_group("Resource" FILES ${RESOURCES})

add_library(QCustomUi SHARED
 ${HEADERS}
 ${PRIVATRE_HEADERS}
 ${LOG_HEADERS}
 ${INPUT_HEADERS}
 ${CONTAINERS_HEADERS}
 ${MESSAGETIP_HEADERS}
 ${DISPLAYWIDGETS_HEADERS}
 ${VIEW_HEADERS}
 ${TOOLS_HEADERS}

 ${SOURCES}
 ${PRIVATRE_SOURCES}
 ${LOG_SOURCES}
 ${CONTAINERS_SOURCES}
 ${INPUT_SOURCES}
 ${MESSAGETIP_SOURCES}
 ${DISPLAYWIDGETS_SOURCES}
 ${VIEW_SOURCES}
 ${TOOLS_SOURCES}

 ${UI}
 ${RESOURCES}
 ${DOCS}
)

target_link_libraries(QCustomUi PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::WinExtras Qt5::Network)
set_target_properties(QCustomUi PROPERTIES VERSION "1.3.0.0" SOVERSION 1)

file(GLOB PUBLIC_HEADERS ./*.h)
set_target_properties(QCustomUi PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

install(TARGETS QCustomUi
    CONFIGURATIONS Debug
    ARCHIVE DESTINATION ${INSTALL_LIBRARYDIR}
    RUNTIME DESTINATION ${INSTALL_BINDIR}/Debug
    PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDEDIR}/QCustomUi
)
install(TARGETS QCustomUi
    CONFIGURATIONS Release
    ARCHIVE DESTINATION ${INSTALL_LIBRARYDIR}
    RUNTIME DESTINATION ${INSTALL_BINDIR}/Release
    PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDEDIR}/QCustomUi
)