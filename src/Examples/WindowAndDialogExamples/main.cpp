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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override
#else
    bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override
#endif
    {
        MSG* msg = (MSG*)message;
        if (msg->message == WM_NCHITTEST)
        {
        }
        return false;
    }
};

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    a.installNativeEventFilter(new MyFilter);
    WindowAndDialogExamples w;
    ww = w.winId();
    w.show();
    return a.exec();
}
