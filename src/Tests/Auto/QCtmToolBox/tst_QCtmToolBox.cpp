#include <QCustomUi/QCtmToolBox.h>

#include <QSignalSpy>
#include <QTest>

class tst_QCtmToolBox : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void addWidget();
    void insertWidget();
    void removeWidget();
    void indexOf();
    void widget();
    void taskGetSet();
    void iconSizeChanged();
    void cleanupTestCase();

private:
    QCtmToolBox* m_toolBox;
    QVector<QWidget*> m_widgets;
};

void tst_QCtmToolBox::initTestCase() { m_toolBox = new QCtmToolBox; }

void tst_QCtmToolBox::addWidget()
{
    auto w1 = new QWidget;
    m_toolBox->addWidget("1", w1);
    auto w2 = new QWidget;
    m_toolBox->addWidget("2", w2);
    m_widgets.push_back(w1);
    m_widgets.push_back(w2);
    widget();
}

void tst_QCtmToolBox::insertWidget()
{
    auto w1 = new QWidget;
    auto w2 = new QWidget;
    m_toolBox->insertWidget(1, "3", w1);
    m_toolBox->insertWidget(0, "4", w2);
    m_widgets.insert(1, w1);
    m_widgets.insert(0, w2);
    widget();
}

void tst_QCtmToolBox::removeWidget()
{
    QPointer<QWidget> w = m_widgets.takeAt(1);
    m_toolBox->removeWidget(w);
    QVERIFY(!w);
}

void tst_QCtmToolBox::indexOf()
{
    for (int i = 0; i < m_widgets.size(); i++)
    {
        QVERIFY(i == m_toolBox->indexOf(m_widgets[i]));
    }
}

void tst_QCtmToolBox::widget()
{
    for (int i = 0; i < m_widgets.size(); i++)
    {
        QVERIFY(m_widgets.at(i) == m_toolBox->widget(i));
    }
}

void tst_QCtmToolBox::taskGetSet()
{
    auto size = m_toolBox->iconSize();
    size.setHeight(size.height() + 1);
    m_toolBox->setIconSize(size);
    QVERIFY(size == m_toolBox->iconSize());
}

void tst_QCtmToolBox::iconSizeChanged()
{
    QSignalSpy spy(m_toolBox, &QCtmToolBox::iconSizeChanged);
    auto size = m_toolBox->iconSize();
    size.setHeight(size.height() + 1);
    m_toolBox->setIconSize(size);
    QVERIFY(spy.count() == 1);
}

void tst_QCtmToolBox::cleanupTestCase() { delete m_toolBox; }

QTEST_MAIN(tst_QCtmToolBox)

#include "tst_QCtmToolBox.moc"
