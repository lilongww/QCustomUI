#include <QCustomUi/QCtmSwitchButton.h>

#include <QSignalSpy>
#include <QTest>

class tst_QCtmSwitchButton : public QObject
{
    Q_OBJECT
private slots:
    void taskGetSet();
};

// 测试Get Set
void tst_QCtmSwitchButton::taskGetSet()
{
    QCtmSwitchButton btn(nullptr);
    {
        QBrush brush(QColor(1, 1, 1));
        btn.setUncheckedBackground(brush);
        QVERIFY(btn.uncheckedBackground() == brush);
    }
    {
        QColor color(2, 2, 2);
        btn.setUncheckedBackgroundBorderColor(color);
        QVERIFY(btn.uncheckedBackgroundBorderColor() == color);
    }
    {
        QBrush brush(QColor(3, 3, 3));
        btn.setUncheckedHandleBrush(brush);
        QVERIFY(btn.uncheckedHandleBrush() == brush);
    }
    {
        QColor color(4, 4, 4);
        btn.setUncheckedHandleBorderColor(color);
        QVERIFY(btn.uncheckedHandleBorderColor() == color);
    }
    {
        QBrush brush(QColor(5, 5, 5));
        btn.setCheckedBackground(brush);
        QVERIFY(btn.checkedBackground() == brush);
    }
    {
        QColor color(6, 6, 6);
        btn.setCheckedBackgroundBorderColor(color);
        QVERIFY(btn.checkedBackgroundBorderColor() == color);
    }
    {
        QBrush brush(QColor(7, 7, 7));
        btn.setCheckedHandleBrush(brush);
        QVERIFY(btn.checkedHandleBrush() == brush);
    }
    {
        QColor color(8, 8, 8);
        btn.setCheckedHandleBorderColor(color);
        QVERIFY(btn.checkedHandleBorderColor() == color);
    }
}

QTEST_MAIN(tst_QCtmSwitchButton)

#include "tst_QCtmSwitchButton.moc"