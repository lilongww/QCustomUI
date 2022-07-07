#include <QCustomUi/QCtmApplication.h>
#include <QCustomUi/QCtmChartBoxLayout.h>

class TestItem : public QCtmChartItem
{
public:
    using QCtmChartItem::QCtmChartItem;
    void draw(QPainter* p) override {}
};

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    {
        auto layout = new QCtmChartBoxLayout(Qt::Vertical, nullptr);
        layout->setGeometry(QRect(0, 0, 1000, 1000));
        layout->setMargin(QMargins(5, 5, 5, 5));
        layout->setSpacing(6);
        for (int i = 0; i < 5; i++)
        {
            layout->addItem(new TestItem(nullptr));
        }
        layout->invalidate();
    }

    {
        auto layout = new QCtmChartBoxLayout(Qt::Vertical, nullptr);
        layout->setGeometry(QRect(0, 0, 1000, 1000));
        layout->setMargin(QMargins(5, 5, 5, 5));
        layout->setSpacing(6);

        auto i1 = new TestItem(nullptr);
        i1->setMinimumSize(QSize(100, 100));
        layout->addItem(i1, 0);
        layout->addItem(new TestItem(nullptr), 1);
        layout->addItem(new TestItem(nullptr), 2);
        auto i2 = new TestItem(nullptr);
        layout->addItem(i2, 3);
        i2->setMaximumSize(QSize(1000, 100));
        layout->invalidate();
    }
    a.exec();
    return 0;
}