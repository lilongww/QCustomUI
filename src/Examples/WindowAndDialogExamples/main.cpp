#include "WindowAndDialogExamples.h"

#include <QCustomUi/QCtmApplication.h>

#include <QAbstractNativeEventFilter>
#include <QDialog>
#include <QDebug>

#include <qt_windows.h>

WId ww;

class MyFilter : public QAbstractNativeEventFilter
{
public:
    bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override
    {
        MSG* msg = (MSG*)message;
        if (msg->message == WM_NCHITTEST)
        {
            if (msg->hwnd == (HWND)ww)
                qDebug() << "get";
        }
        return false;
    }
};

int main(int argc, char* argv[])
{
    //当QCtmWindow的子控件取了winId或者设置了Qt::WA_NativeWindow，顶层的WM_NCHITTEST无法收到，设置该属性可以解决该问题
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QCtmApplication a(argc, argv);
    a.installNativeEventFilter(new MyFilter);
    WindowAndDialogExamples w;
    ww = w.winId();
    w.show();
    return a.exec();
}
