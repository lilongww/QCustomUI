#include <QCustomUi/QCtmWindow.h>

#include <QScreen>
#include <QSignalSpy>
#include <QTest>
#include <QVBoxLayout>

class tst_QCtmWindow : public QObject
{
    Q_OBJECT
private slots:
    void fixedSizeWidgetShowMaximumBug();
    void taskStatusBar();
};

void tst_QCtmWindow::fixedSizeWidgetShowMaximumBug()
{
    QCtmWindow w;
    auto widget = new QWidget(&w);
    widget->setFixedSize(650, 260);
    auto layout = new QVBoxLayout(w.centralWidget());
    layout->addWidget(widget);
    w.showMaximized();
    qApp->processEvents();
    QVERIFY(QTest::qWaitForWindowExposed(&w));
    QCOMPARE(w.size(), w.screen()->availableSize());
}

QTEST_MAIN(tst_QCtmWindow)

void tst_QCtmWindow::taskStatusBar()
{
    QCtmWindow w;
    auto statusBar = w.statusBar();
    QVERIFY(statusBar);
    delete statusBar;
    statusBar = new QStatusBar;
    w.setStatusBar(statusBar);
    QVERIFY(statusBar == w.statusBar());
    w.setStatusBar(nullptr);
    QVERIFY(statusBar != w.statusBar());
}

#include "tst_QCtmWindow.moc"
