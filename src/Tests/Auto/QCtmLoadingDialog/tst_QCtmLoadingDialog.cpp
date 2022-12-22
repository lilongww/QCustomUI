#include <QCustomUi/QCtmLoadingDialog.h>

#include <QSignalSpy>
#include <QTest>
#include <QTimer>

#include <chrono>

using namespace std::chrono_literals;

class tst_QCtmLoadingDialog : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cancelEnable();
    void setMovie();
    void cleanupTestCase();

private:
    QWidget* m_topWidget;
    QCtmLoadingDialog* m_loading;
};

void tst_QCtmLoadingDialog::initTestCase()
{
    m_topWidget = new QWidget;
    m_topWidget->show();
    QVERIFY(QTest::qWaitForWindowExposed(m_topWidget));
    m_loading = new QCtmLoadingDialog(m_topWidget);
}

void tst_QCtmLoadingDialog::cancelEnable()
{
    { //  get set
        auto flag = m_loading->cancelEnable();
        m_loading->setCancelEnable(!flag);
        QVERIFY(flag != m_loading->cancelEnable());
    }

    { // cancel enable true
        m_loading->setCancelEnable(true);
        QTimer::singleShot(1s, m_loading, [=]() { QTest::keyClick(m_loading, Qt::Key_Escape); });
        QVERIFY(m_loading->exec() == QCtmLoadingDialog::Result::Cancel);
    }

    { // cancel enable false
        m_loading->setCancelEnable(false);
        QTimer::singleShot(1s, m_loading, [=]() { QTest::keyClick(m_loading, Qt::Key_Escape); });
        QTimer::singleShot(2s, m_loading, [=]() { m_loading->done(); });
        QVERIFY(m_loading->exec() == QCtmLoadingDialog::Result::Done);
    }
}

void tst_QCtmLoadingDialog::setMovie()
{
    auto movie = new QMovie;
    m_loading->setMovie(movie);
    QVERIFY(m_loading->movie() == movie);
}

void tst_QCtmLoadingDialog::cleanupTestCase() { delete m_topWidget; }

QTEST_MAIN(tst_QCtmLoadingDialog)

#include "tst_QCtmLoadingDialog.moc"
