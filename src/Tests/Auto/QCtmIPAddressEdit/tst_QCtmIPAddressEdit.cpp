#include <QCustomUi/QCtmIPAddressEdit.h>

#include <QObject>
#include <QTest>

class tst_QCtmIPAddressEdit : public QObject
{
    Q_OBJECT
private slots:
    void testSetGet();
    void testInvalidIP();
    void testInput();
};

// 测试设置和获取
void tst_QCtmIPAddressEdit::testSetGet()
{
    QString ip("192.168.0.1");
    QCtmIPAddressEdit edit(nullptr);
    edit.setIPAddress(ip);
    QVERIFY(edit.ipAddress() == ip);
}
// 测试非法IP
void tst_QCtmIPAddressEdit::testInvalidIP()
{
    QCtmIPAddressEdit edit(nullptr);
    edit.setIPAddress("1.1.555.1");
    QVERIFY(edit.ipAddress() == "0.0.0.0");
    edit.setIPAddress("1.1.1.1.1");
    QVERIFY(edit.ipAddress() == "0.0.0.0");
}
// 输入测试
void tst_QCtmIPAddressEdit::testInput()
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

// main
QTEST_MAIN(tst_QCtmIPAddressEdit)

#include "tst_QCtmIPAddressEdit.moc"