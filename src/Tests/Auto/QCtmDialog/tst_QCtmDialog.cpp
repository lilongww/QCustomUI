#include <QCustomUi/QCtmDialog.h>
#include <QCustomUi/QCtmTitleBar.h>

#include <QScreen>
#include <QSignalSpy>
#include <QTest>
#include <QVBoxLayout>

class tst_QCtmDialog : public QObject
{
    Q_OBJECT
private slots:
    void taskTitleBar();
};

void tst_QCtmDialog::taskTitleBar()
{
    QCtmDialog w;
    QPointer<QCtmTitleBar> oldbar = w.titleBar();
    delete oldbar;
    auto bar = w.titleBar();
    QVERIFY(bar != oldbar);
    bar = new QCtmTitleBar(&w);
    w.setTitleBar(bar);
    QVERIFY(bar == w.titleBar());
    w.setTitleBar(nullptr);
    QVERIFY(bar != w.titleBar());
}

QTEST_MAIN(tst_QCtmDialog)

#include "tst_QCtmDialog.moc"
