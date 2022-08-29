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

#include "tst_QCtmWindow.moc"
