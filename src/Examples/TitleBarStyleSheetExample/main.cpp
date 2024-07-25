#include <QCustomUi/QCtmApplication.h>
#include <QCustomUi/QCtmStyleSheet.h>
#include <QCustomUi/QCtmWindow.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv, QCtmApplication::ApplicationFlags, false);
    auto style = QCtmStyleSheet::defaultStyleSheet();
    style += "QCtmTitleBar"
             "{"
             "   background: #1F1F1F;"
             "   color: white;"
             "}"
             "QCtmTitleBar QAbstractButton:hover,QCtmTitleBar QAbstractButton:pressed"
             "{"
             "   background: #3D3D3D;"
             "}"
             "QCtmTitleBar #closeBtn:hover,QCtmTitleBar #closeBtn:pressed"
             "{"
             "   background: #C32B1C;"
             "}"
             "QCtmTitleBar #closeBtn"
             "{"
             "   qproperty-icon: url(:/QCustomUi/Resources/close.png);"
             "}"
             "QCtmTitleBar QAbstractButton:hover,QCtmTitleBar QAbstractButton:pressed"
             "{"
             "   background: #3D3D3D;"
             "}"
             "QCtmMaximumSizeButton"
             "{"
             "   qproperty-maximumSizedIcon: url(:/QCustomUi/Resources/maximize.png);"
             "   qproperty-revertIcon: url(:/QCustomUi/Resources/maximize_max.png);"
             "}"
             "QCtmTitleBar #minimumSizeBtn"
             "{"
             "   qproperty-icon: url(:/QCustomUi/Resources/minimize.png);"
             "}";
    a.setStyleSheet(style);
    QCtmWindow w;
    w.show();
    return a.exec();
}
