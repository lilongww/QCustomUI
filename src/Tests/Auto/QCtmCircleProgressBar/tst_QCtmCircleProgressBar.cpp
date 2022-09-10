#include <QCustomUi/QCtmCircleProgressBar.h>

#include <QEventLoop>
#include <QSignalSpy>
#include <QTest>
#include <QTimer>

class tst_QCtmCircleProgressBar : public QObject
{
    Q_OBJECT
private slots:
    void taskGetSet();
    void minMaxSameValue();
    void text();
    void textVisible();
    void format();
    void penCapStyle();
    void barWidth();
    void barBrush();
    void backgroundBrush();
    void valueChanged();
};

class CircleProgressBar : public QCtmCircleProgressBar
{
public:
    using QCtmCircleProgressBar::QCtmCircleProgressBar;

protected:
    void paintEvent(QPaintEvent* event) override
    {
        m_repainted = true;
        QCtmCircleProgressBar::paintEvent(event);
    }

public:
    bool m_repainted { false };
};

void tst_QCtmCircleProgressBar::taskGetSet()
{
    QCtmCircleProgressBar bar(nullptr);
    bar.setMinimum(0);
    QCOMPARE(bar.minimum(), 0);
    bar.setMinimum(INT_MAX);
    QCOMPARE(bar.minimum(), INT_MAX);
    bar.setMinimum(INT_MIN);
    QCOMPARE(bar.minimum(), INT_MIN);

    bar.setMaximum(0);
    QCOMPARE(bar.maximum(), 0);
    bar.setMaximum(INT_MIN);
    QCOMPARE(bar.maximum(), INT_MIN);
    bar.setMaximum(INT_MAX);
    QCOMPARE(bar.maximum(), INT_MAX);

    bar.setValue(0);
    QCOMPARE(bar.value(), 0);
    bar.setValue(INT_MIN);
    QCOMPARE(bar.value(), INT_MIN);
    bar.setValue(INT_MAX);
    QCOMPARE(bar.value(), INT_MAX);
}

void tst_QCtmCircleProgressBar::minMaxSameValue()
{
    QCtmCircleProgressBar bar(nullptr);
    bar.setRange(10, 10);
    bar.setValue(10);
    bar.move(300, 300);
    bar.show();
    QVERIFY(QTest::qWaitForWindowExposed(&bar));
}

void tst_QCtmCircleProgressBar::text()
{
    QCtmCircleProgressBar bar(nullptr);
    bar.setRange(10, 10);
    bar.setValue(10);
    QCOMPARE(bar.text(), QString("100%"));
    bar.setRange(0, 10);
    QCOMPARE(bar.text(), QString("100%"));
    bar.setValue(5);
    QCOMPARE(bar.text(), QString("50%"));
    bar.setRange(0, 5);
    bar.setValue(0);
    bar.setRange(5, 5);
    QCOMPARE(bar.text(), QString());
}

void tst_QCtmCircleProgressBar::textVisible()
{
    CircleProgressBar bar(nullptr);
    bar.show();
    QVERIFY(QTest::qWaitForWindowExposed(&bar));
    bar.setTextVisible(true);
    QCOMPARE(bar.isTextVisible(), true);
    qApp->processEvents();
    QCOMPARE(bar.m_repainted, true);
    bar.m_repainted = false;
    bar.setTextVisible(false);
    QCOMPARE(bar.isTextVisible(), false);
    qApp->processEvents();
    QCOMPARE(bar.m_repainted, true);
}

void tst_QCtmCircleProgressBar::format()
{
    CircleProgressBar bar(nullptr);
    bar.setRange(0, 10);
    bar.setValue(1);
    bar.move(300, 300);
    bar.show();
    QVERIFY(QTest::qWaitForWindowExposed(&bar));

    bar.m_repainted = false;
    bar.setFormat("%v of %m (%p%)");
    qApp->processEvents();
    QTRY_VERIFY(bar.m_repainted);
    bar.m_repainted = false;
    bar.setFormat("%v of %m (%p%)");
    qApp->processEvents();

    QCOMPARE(bar.text(), QString("1 of 10 (10%)"));
    bar.setRange(5, 5);
    bar.setValue(5);
    QCOMPARE(bar.text(), QString("5 of 0 (100%)"));
    bar.setRange(0, 5);
    bar.setValue(0);
    bar.setRange(5, 5);
    QCOMPARE(bar.text(), QString());
}

void tst_QCtmCircleProgressBar::penCapStyle()
{
    CircleProgressBar bar { nullptr };
    bar.setPenCapStyle(Qt::RoundCap);
    QCOMPARE(bar.penCapStyle(), Qt::RoundCap);
    bar.setPenCapStyle(Qt::FlatCap);
    QCOMPARE(bar.penCapStyle(), Qt::FlatCap);
}

void tst_QCtmCircleProgressBar::barWidth()
{
    CircleProgressBar bar { nullptr };
    bar.setBarWidth(5);
    QCOMPARE(bar.barWidth(), 5);
    bar.setBarWidth(1);
    QCOMPARE(bar.barWidth(), 1);
}

void tst_QCtmCircleProgressBar::barBrush()
{
    CircleProgressBar bar { nullptr };
    QBrush brush1(Qt::red);
    QBrush brush2(Qt::green);
    bar.setBarBrush(brush1);
    QCOMPARE(bar.barBrush(), brush1);
    bar.setBarBrush(brush2);
    QCOMPARE(bar.barBrush(), brush2);
}

void tst_QCtmCircleProgressBar::backgroundBrush()
{
    CircleProgressBar bar { nullptr };
    QBrush brush1(Qt::red);
    QBrush brush2(Qt::green);
    bar.setBackgroundBrush(brush1);
    QCOMPARE(bar.backgroundBrush(), brush1);
    bar.setBackgroundBrush(brush2);
    QCOMPARE(bar.backgroundBrush(), brush2);
}

void tst_QCtmCircleProgressBar::valueChanged()
{
    CircleProgressBar bar { nullptr };
    QSignalSpy spy(&bar, &CircleProgressBar::valueChanged);
    bar.setValue(11);
    bar.setValue(12);
    bar.setValue(12);
    QCOMPARE(spy.count(), 2);
}

QTEST_MAIN(tst_QCtmCircleProgressBar)

#include "tst_QCtmCircleProgressBar.moc"
