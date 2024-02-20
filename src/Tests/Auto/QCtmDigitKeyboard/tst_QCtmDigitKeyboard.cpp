#include <QCustomUi/QCtmDigitKeyboard.h>

#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QScopeGuard>
#include <QScreen>
#include <QSignalSpy>
#include <QSpinBox>
#include <QTest>

class tst_QCtmDigitKeyboard : public QObject
{
    Q_OBJECT
private slots:
    void bindSpinBox();
    void bindDoubleSpinBox();
    void spinBoxRangeTest();
    void doubleSpinBoxRangeTest();
    void disableTest();
    void readOnlyTest();

private:
    QPushButton* findButtonHasText(QCtmDigitKeyboard* board, const QString& txt) const
    {
        auto btns = board->findChildren<QPushButton*>();
        auto it   = std::find_if(btns.begin(), btns.end(), [&](auto btn) { return btn->text() == txt; });
        return it == btns.end() ? nullptr : *it;
    }
};

void tst_QCtmDigitKeyboard::bindSpinBox()
{
    QSpinBox box(nullptr);
    auto board = new QCtmDigitKeyboard(&box);
    board->setUnits({ { "Hz", 0, 1000'000 }, { "kHz", 0, 1000 } });
    board->bindBox(&box);
    box.show();
    QVERIFY(QTest::qWaitForWindowExposed(&box));

    QMetaObject::invokeMethod(
        board,
        [&]()
        {
            auto scope = qScopeGuard(
                [&]
                {
                    box.close();
                    board->hide();
                });
            QVERIFY(QTest::qWaitForWindowActive(board));
            QVERIFY(findButtonHasText(board, "Hz"));
            auto khzBtn = findButtonHasText(board, "kHz");
            QVERIFY(khzBtn);
            QVERIFY(!findButtonHasText(board, ".")->isVisible());
            auto boardBox = board->findChild<QLineEdit*>();
            QTest::keyEvent(QTest::Click, boardBox, Qt::Key_1);
            QTest::keyEvent(QTest::Click, boardBox, Qt::Key_2);
            QTest::mouseClick(khzBtn, Qt::MouseButton::LeftButton);
        },
        Qt::QueuedConnection);
    QTest::mouseClick(box.findChild<QLineEdit*>(), Qt::MouseButton::LeftButton);
    QVERIFY(!board->isVisible());
    QVERIFY(box.minimum() == 0);
    QVERIFY(box.maximum() == 1000);
    QVERIFY(box.suffix() == "kHz");
    QVERIFY(box.value() == 12);
}

void tst_QCtmDigitKeyboard::bindDoubleSpinBox()
{
    QDoubleSpinBox box(nullptr);
    auto board = new QCtmDigitKeyboard(&box);
    board->setUnits({ { "Hz", 0, 1E6 }, { "kHz", 0, 1E3 } });
    board->bindBox(&box);
    box.show();
    QVERIFY(QTest::qWaitForWindowExposed(&box));

    QMetaObject::invokeMethod(
        board,
        [&]()
        {
            auto scope = qScopeGuard(
                [&]
                {
                    box.close();
                    board->hide();
                });
            QVERIFY(QTest::qWaitForWindowActive(board));
            QVERIFY(findButtonHasText(board, "Hz"));
            auto khzBtn = findButtonHasText(board, "kHz");
            QVERIFY(khzBtn);
            QVERIFY(findButtonHasText(board, ".")->isVisible());
            auto boardBox = board->findChild<QLineEdit*>();
            QTest::keyEvent(QTest::Click, boardBox, Qt::Key_1);
            QTest::keyEvent(QTest::Click, boardBox, Qt::Key_3);
            QTest::mouseClick(khzBtn, Qt::MouseButton::LeftButton);
        },
        Qt::QueuedConnection);
    QTest::mouseClick(box.findChild<QLineEdit*>(), Qt::MouseButton::LeftButton);
    QVERIFY(!board->isVisible());
    QVERIFY(box.minimum() == 0);
    QVERIFY(box.maximum() == 1E3);
    QVERIFY(box.suffix() == "kHz");
    QVERIFY(box.value() - 13.0 < std::numeric_limits<double>::min());
}

void tst_QCtmDigitKeyboard::spinBoxRangeTest()
{
    {
        QSpinBox box(nullptr);
        auto board = new QCtmDigitKeyboard(&box);
        board->setUnits({ { "Hz", 0, 1000'000 }, { "kHz", 0, 1000 } });
        board->bindBox(&box);
        QVERIFY(box.minimum() == 0 && box.maximum() == 1000'000);
    }
    {
        QSpinBox box(nullptr);
        auto board = new QCtmDigitKeyboard(&box);
        board->bindBox(&box);
        QVERIFY(box.minimum() == 0 && box.maximum() != 1000'000);
        board->setUnits({ { "Hz", 0, 1000'000 }, { "kHz", 0, 1000 } });
        QVERIFY(box.minimum() == 0 && box.maximum() == 1000'000);
    }
    {
        QSpinBox box(nullptr);
        auto board = new QCtmDigitKeyboard(&box);
        box.setSuffix("kHz");
        board->bindBox(&box);
        board->setUnits({ { "Hz", 0, 1000'000 }, { "kHz", 0, 1000 } });
        QVERIFY(box.minimum() == 0 && box.maximum() == 1000);
    }
}

void tst_QCtmDigitKeyboard::doubleSpinBoxRangeTest()
{
    {
        QDoubleSpinBox box(nullptr);
        auto board = new QCtmDigitKeyboard(&box);
        board->setUnits({ { "Hz", 0.0, 1E6 }, { "kHz", 0.0, 1E3 } });
        board->bindBox(&box);
        QVERIFY(box.minimum() == 0.0 && box.maximum() == 1E6);
    }
    {
        QDoubleSpinBox box(nullptr);
        auto board = new QCtmDigitKeyboard(&box);
        board->bindBox(&box);
        QVERIFY(box.minimum() == 0.0 && box.maximum() != 1E6);
        board->setUnits({ { "Hz", 0.0, 1E6 }, { "kHz", 0.0, 1E3 } });
        QVERIFY(box.minimum() == 0.0 && box.maximum() == 1E6);
    }
    {
        QDoubleSpinBox box(nullptr);
        auto board = new QCtmDigitKeyboard(&box);
        box.setSuffix("kHz");
        board->bindBox(&box);
        board->setUnits({ { "Hz", 0.0, 1E6 }, { "kHz", 0.0, 1E3 } });
        QVERIFY(box.minimum() == 0.0 && box.maximum() == 1E3);
    }
}

void tst_QCtmDigitKeyboard::disableTest()
{
    QSpinBox box(nullptr);
    auto board = new QCtmDigitKeyboard(&box);
    board->bindBox(&box);
    box.setEnabled(false);
    box.show();
    QVERIFY(QTest::qWaitForWindowExposed(&box));
    QMetaObject::invokeMethod(
        board,
        [&]()
        {
            auto scope = qScopeGuard(
                [&]
                {
                    box.close();
                    board->hide();
                });
            QVERIFY(!QTest::qWaitFor([=] { return board->isVisible(); }, 1000));
        },
        Qt::QueuedConnection);
    QTest::mouseClick(box.findChild<QLineEdit*>(), Qt::MouseButton::LeftButton);
}

void tst_QCtmDigitKeyboard::readOnlyTest()
{
    QSpinBox box(nullptr);
    auto board = new QCtmDigitKeyboard(&box);
    board->bindBox(&box);
    box.setReadOnly(true);
    box.show();
    QVERIFY(QTest::qWaitForWindowExposed(&box));
    QMetaObject::invokeMethod(
        board,
        [&]()
        {
            auto scope = qScopeGuard(
                [&]
                {
                    box.close();
                    board->hide();
                });
            QVERIFY(!QTest::qWaitFor([=] { return board->isVisible(); }, 1000));
        },
        Qt::QueuedConnection);
    QTest::mouseClick(box.findChild<QLineEdit*>(), Qt::MouseButton::LeftButton);
}

QTEST_MAIN(tst_QCtmDigitKeyboard)

#include "tst_QCtmDigitKeyboard.moc"
