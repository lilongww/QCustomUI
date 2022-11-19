#include <QCustomUi/QCtmIPAddressEdit.h>

#include <QObject>
#include <QSignalSpy>
#include <QTest>

class tst_QCtmIPAddressEdit : public QObject
{
    Q_OBJECT
private slots:
    void taskSetGet();
    void taskInvalidIP();
    void taskInput();
    void taskCursor();
    void taskReadOnly();
    void taskEditChanged();
    void taskEditFinished();
};

// 测试设置和获取
void tst_QCtmIPAddressEdit::taskSetGet()
{
    QString ip("192.168.0.1");
    QCtmIPAddressEdit edit(nullptr);
    edit.setIPAddress(ip);
    QVERIFY(edit.ipAddress() == ip);
}
// 测试非法IP
void tst_QCtmIPAddressEdit::taskInvalidIP()
{
    QCtmIPAddressEdit edit(nullptr);
    edit.setIPAddress("1.1.555.1");
    QVERIFY(edit.ipAddress() == "0.0.0.0");
    edit.setIPAddress("1.1.1.1.1");
    QVERIFY(edit.ipAddress() == "0.0.0.0");
}
// 输入测试
void tst_QCtmIPAddressEdit::taskInput()
{
    {
        QCtmIPAddressEdit edit(nullptr);
        edit.show();
        QTest::keyClicks(&edit, "192.168.1.1");
        QVERIFY(edit.ipAddress() == "192.168.1.1");
        QTest::keyClick(&edit, Qt::Key_Backspace);
        QTest::keyClick(&edit, Qt::Key_Backspace);
        QTest::keyClick(&edit, Qt::Key_Backspace);
        QTest::keyClicks(&edit, "1.1.1");
        QVERIFY(edit.ipAddress() == "192.161.1.1");
    }
    {
        QCtmIPAddressEdit edit(nullptr);
        edit.show();
        QTest::keyClicks(&edit, "1921681.1");
        QVERIFY(edit.ipAddress() == "192.168.1.1");
    }
    {
        QCtmIPAddressEdit edit(nullptr);
        edit.show();
        QTest::keyClicks(&edit, "19A21D6F81.1");
        QVERIFY(edit.ipAddress() == "192.168.1.1");
    }
}

// 测试游标
void tst_QCtmIPAddressEdit::taskCursor()
{
    QCtmIPAddressEdit edit(nullptr);
    edit.show();
    QTest::keyClick(&edit, Qt::Key_Right);
    QTest::keyClicks(&edit, "1.1.1");
    QVERIFY(edit.ipAddress() == "0.1.1.1");
}
// 测试只读属性
void tst_QCtmIPAddressEdit::taskReadOnly()
{
    QCtmIPAddressEdit edit(nullptr);
    edit.show();
    edit.setReadOnly(true);
    QTest::keyClicks(&edit, "1.1.1.1");
    QVERIFY(edit.ipAddress() == "0.0.0.0");
    edit.setReadOnly(false);
    QTest::keyClicks(&edit, "1.1.1.1");
    QVERIFY(edit.ipAddress() == "1.1.1.1");
}

// 测试editChanged信号
void tst_QCtmIPAddressEdit::taskEditChanged()
{
    QCtmIPAddressEdit edit(nullptr);
    edit.show();
    QSignalSpy spy(&edit, SIGNAL(editChanged()));
    QTest::keyClicks(&edit, "192.168.1.1");
    QVERIFY(spy.count() == 8);
}

// 测试editFinished信号
void tst_QCtmIPAddressEdit::taskEditFinished()
{
    QWidget t(nullptr);
    QCtmIPAddressEdit edit(&t);
    QCtmIPAddressEdit edit2(&t);
    t.show();
    edit.setFocus();
    QSignalSpy spy(&edit, SIGNAL(editingFinished()));
    QTest::keyClicks(&edit, "192.168.1.1");
    QTest::keyClick(&edit, Qt::Key_Enter);
    QTest::keyClick(&edit, Qt::Key_Return);
    QVERIFY(spy.count() == 2);
    edit2.setFocus();
    QTest::keyClicks(&edit2, "192.168.1.1");
    QSignalSpy spy2(&edit2, SIGNAL(editingFinished()));
    edit.setFocus();
    QVERIFY(spy2.count() == 1);
}

// main
QTEST_MAIN(tst_QCtmIPAddressEdit)

#include "tst_QCtmIPAddressEdit.moc"