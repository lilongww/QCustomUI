#include <QCustomUi/QCtmStageProgressBar.h>

#include <QEventLoop>
#include <QSignalSpy>
#include <QTest>
#include <QTimer>

class tst_QCtmStageProgressBar : public QObject
{
    Q_OBJECT
private slots:
    void taskGetSet();
    void taskValueChanged();
};

class StageProgressBar : public QCtmStageProgressBar
{
public:
    using QCtmStageProgressBar::QCtmStageProgressBar;

protected:
    void paintEvent(QPaintEvent* event) override
    {
        m_repainted = true;
        QCtmStageProgressBar::paintEvent(event);
    }

public:
    bool m_repainted { false };
};

void tst_QCtmStageProgressBar::taskGetSet()
{
    QCtmStageProgressBar bar(nullptr);
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

    bar.setOrientation(Qt::Horizontal);
    QCOMPARE(bar.orientation(), Qt::Horizontal);
    bar.setOrientation(Qt::Vertical);
    QCOMPARE(bar.orientation(), Qt::Vertical);

    bar.setStageCount(2);
    QCOMPARE(bar.stageCount(), 2);
    bar.setStageCount(1);
    QCOMPARE(bar.stageCount(), 2);

    bar.setStageCircleRadius(5);
    QCOMPARE(bar.stageCircleRadius(), 5);
    bar.setStageCircleRadius(6);
    QCOMPARE(bar.stageCircleRadius(), 6);

    for (int i = 0; i < 2; i++)
    {
        auto txt = QString::number(i);
        bar.setText(i, txt);
        QCOMPARE(bar.text(i), txt);
    }

    bar.setTextVisible(false);
    QCOMPARE(bar.textVisible(), false);
    bar.setTextVisible(true);
    QCOMPARE(bar.textVisible(), true);

    QPen pen(Qt::red);
    bar.setStageIndexTextPen(pen);
    QCOMPARE(bar.stageIndexTextPen(), pen);
    QBrush brush(Qt::blue);
    bar.setRateBackground(brush);
    QCOMPARE(bar.rateBackground(), brush);
}

void tst_QCtmStageProgressBar::taskValueChanged()
{
    QCtmStageProgressBar bar(nullptr);
    QSignalSpy spy(&bar, &QCtmStageProgressBar::valueChanged);
    bar.setValue(5);
    QCOMPARE(spy.count(), 1);
    bar.setMaximum(40);
    QCOMPARE(spy.count(), 1);
    bar.setMaximum(4);
    QCOMPARE(spy.count(), 2);
    bar.setMinimum(1);
    QCOMPARE(spy.count(), 2);
    bar.setMinimum(5);
    QCOMPARE(spy.count(), 3);
}

QTEST_MAIN(tst_QCtmStageProgressBar)

#include "tst_QCtmStageProgressBar.moc"
