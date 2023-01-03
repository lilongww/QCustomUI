#include <QCustomUi/QCtmEditLabel.h>

#include <QLineEdit>
#include <QSignalSpy>
#include <QStyle>
#include <QTest>
#include <QToolButton>

class tst_QCtmEditLabel : public QObject
{
    Q_OBJECT
private slots:
    void taskGetSet();
    void editFinished();
};

void tst_QCtmEditLabel::taskGetSet()
{
    QCtmEditLabel label;
    auto readOnly = label.isReadOnly();
    label.setReadOnly(!readOnly);
    QVERIFY(readOnly != label.isReadOnly());

    {
        auto icon = label.style()->standardIcon(QStyle::SP_ArrowBack);
        label.setEditButtonIcon(icon);
        QVERIFY(!label.editButtonIcon().isNull());
    }

    {
        auto icon = label.style()->standardIcon(QStyle::SP_ArrowDown);
        label.setFinishButtonIcon(icon);
        QVERIFY(!label.finishButtonIcon().isNull());
    }

    {
        auto icon = label.style()->standardIcon(QStyle::SP_ArrowForward);
        label.setRevertButtonIcon(icon);
        QVERIFY(!label.revertButtonIcon().isNull());
    }
}

void tst_QCtmEditLabel::editFinished()
{
    QCtmEditLabel label;
    QSignalSpy spy(&label, &QCtmEditLabel::editFinished);
    label.show();
    QVERIFY(QTest::qWaitForWindowExposed(&label));
    QMetaObject::invokeMethod(&label, "onEditButtonClicked");
    auto input = label.findChild<QLineEdit*>();
    QVERIFY(input);
    QTest::sendKeyEvent(QTest::KeyAction::Click, input, Qt::Key_0, QTest::keyToAscii(Qt::Key_0), Qt::KeyboardModifier::NoModifier);
    auto applyBtn = label.findChild<QToolButton*>("QCtmEditLabel_applyBtn");
    QTest::mouseClick(applyBtn, Qt::LeftButton);
    QVERIFY(spy.size() == 1);
}

QTEST_MAIN(tst_QCtmEditLabel)

#include "tst_QCtmEditLabel.moc"
