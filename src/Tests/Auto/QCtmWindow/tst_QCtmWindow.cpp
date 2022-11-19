#include <QCustomUi/QCtmNavigationBar.h>
#include <QCustomUi/QCtmTitleBar.h>
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
    void taskMenuBar();
    void taskNavigationBar();
    void taskTitleBar();
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

void tst_QCtmWindow::taskMenuBar()
{
    QCtmWindow w;
    auto menuBar = w.menuBar();
    QVERIFY(menuBar);
    delete menuBar;
    menuBar = new QMenuBar;
    w.setMenuBar(menuBar);
    QVERIFY(menuBar == w.menuBar());
    w.setMenuBar(nullptr);
    QVERIFY(menuBar != w.menuBar());
}

void tst_QCtmWindow::taskNavigationBar()
{
    QCtmWindow w;
    auto navigationBar = w.navigationBar();
    QVERIFY(navigationBar);
    delete navigationBar;
    navigationBar = new QCtmNavigationBar(&w);
    w.setNavigationBar(navigationBar);
    QVERIFY(navigationBar == w.navigationBar());
    w.setNavigationBar(nullptr);
    QVERIFY(navigationBar != w.navigationBar());
}

void tst_QCtmWindow::taskTitleBar()
{
    QCtmWindow w;
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

QTEST_MAIN(tst_QCtmWindow)

#include "tst_QCtmWindow.moc"
