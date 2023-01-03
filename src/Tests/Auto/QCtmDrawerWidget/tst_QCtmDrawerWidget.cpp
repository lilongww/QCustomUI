#include <QCustomUi/QCtmDrawerItemWidget.h>
#include <QCustomUi/QCtmDrawerWidget.h>

#include <QSignalSpy>
#include <QTest>
#include <QVector>

class tst_QCtmDrawerWidget : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void addWidget();
    void insertWidget();
    void removeItem();
    void indexOf();
    void item();
    void count();
    void taskGetSet();
    void itemExpandChanged();
    void itemTitleClicked();
    void itemSizeChanged();
    void expandAll();
    void collapseAll();
    void taskExclusive();
    void cleanupTestCase();

private:
    QCtmDrawerWidget* m_drawerWidget;
    QVector<QCtmDrawerItemWidget*> m_items;
};

void tst_QCtmDrawerWidget::initTestCase() { m_drawerWidget = new QCtmDrawerWidget; }

void tst_QCtmDrawerWidget::addWidget()
{
    m_items.push_back(m_drawerWidget->addWidget("1", new QLabel("1")));
    m_items.push_back(m_drawerWidget->addWidget("2", new QLabel("2")));
    item();
}

void tst_QCtmDrawerWidget::insertWidget()
{
    m_items.insert(1, m_drawerWidget->insertWidget(1, "3", new QLabel("3")));
    m_items.insert(0, m_drawerWidget->insertWidget(0, "4", new QLabel("4")));
    item();
}

void tst_QCtmDrawerWidget::removeItem()
{
    QPointer<QCtmDrawerItemWidget> item = m_items.takeAt(1);
    m_drawerWidget->removeItem(item);
    QVERIFY(!item);
}

void tst_QCtmDrawerWidget::indexOf()
{
    auto item = m_items.at(1);
    QVERIFY(1 == m_drawerWidget->indexOf(item));
}

void tst_QCtmDrawerWidget::item()
{
    for (int i = 0; i < m_items.size(); i++)
    {
        QVERIFY(m_items.at(i) == m_drawerWidget->item(i));
    }
}

void tst_QCtmDrawerWidget::count() { QVERIFY(m_drawerWidget->count() == m_items.size()); }

void tst_QCtmDrawerWidget::taskGetSet()
{
    {
        auto e = m_drawerWidget->exclusive();
        m_drawerWidget->setExclusive(!e);
        QVERIFY(e != m_drawerWidget->exclusive());
    }

    {
        auto iconSize = m_drawerWidget->iconSize();
        iconSize.setHeight(iconSize.height() + 1);
        m_drawerWidget->setIconSize(iconSize);
        QVERIFY(iconSize == m_drawerWidget->iconSize());
    }
}

void tst_QCtmDrawerWidget::itemExpandChanged()
{
    m_drawerWidget->setExclusive(false);
    QSignalSpy spy(m_drawerWidget, &QCtmDrawerWidget::itemExpandChanged);
    for (auto item : m_items)
    {
        item->setExpand(!item->isExpand());
    }
    QVERIFY(m_items.size() == spy.count());
}

void tst_QCtmDrawerWidget::itemTitleClicked()
{
    QSignalSpy spy(m_drawerWidget, &QCtmDrawerWidget::itemTitleClicked);
    for (auto item : m_items)
    {
        auto title = item->findChild<QPushButton*>();
        QVERIFY(title && std::strcmp("QCtmDrawerItemTitle", title->metaObject()->className()) == 0);
        QTest::mouseClick(title, Qt::LeftButton);
    }
    QVERIFY(m_items.size() == spy.count());
}

void tst_QCtmDrawerWidget::itemSizeChanged()
{
    QSignalSpy spy(m_drawerWidget, &QCtmDrawerWidget::iconSizeChanged);
    auto iconSize = m_drawerWidget->iconSize();
    iconSize.setHeight(iconSize.height() + 1);
    m_drawerWidget->setIconSize(iconSize);
    QVERIFY(spy.count() == 1);
}

void tst_QCtmDrawerWidget::expandAll()
{
    for (auto item : m_items)
    {
        item->setExpand(false);
    }
    QSignalSpy spy(m_drawerWidget, &QCtmDrawerWidget::itemExpandChanged);
    m_drawerWidget->expandAll();
    QVERIFY(spy.count() == m_items.size());
    for (auto item : m_items)
    {
        QVERIFY(item->isExpand());
    }
}

void tst_QCtmDrawerWidget::collapseAll()
{
    QSignalSpy spy(m_drawerWidget, &QCtmDrawerWidget::itemExpandChanged);
    m_drawerWidget->collapseAll();
    QVERIFY(spy.count() == m_items.size());
    for (auto item : m_items)
    {
        QVERIFY(!item->isExpand());
    }
}

void tst_QCtmDrawerWidget::taskExclusive()
{
    m_drawerWidget->setExclusive(true);
    for (auto item : m_items)
    {
        item->setExpand(true);
        for (auto i : m_items)
        {
            bool expand = i->isExpand();
            QVERIFY(i == item ? expand : !expand);
        }
    }
}

void tst_QCtmDrawerWidget::cleanupTestCase() { delete m_drawerWidget; }

QTEST_MAIN(tst_QCtmDrawerWidget)

#include "tst_QCtmDrawerWidget.moc"
