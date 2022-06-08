#pragma once

#include "qcustomui_global.h"

#include <QPen>
#include <QWidget>

class QCtmAbstractSeries;
class QCUSTOMUI_EXPORT QCtmAbstractChartView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QFont titleFont READ titleFont WRITE setTitleFont)
    Q_PROPERTY(QPen titlePen READ titlePen WRITE setTitlePen)
public:
    QCtmAbstractChartView(QWidget* parent);
    ~QCtmAbstractChartView();

    void setTitle(const QString& title);
    const QString& title() const;
    void setTitleFont(const QFont& font);
    const QFont& titleFont() const;
    void setTitlePen(const QPen& pen);
    const QPen& titlePen() const;
    virtual void addSeries(QCtmAbstractSeries* series);
    const QList<QCtmAbstractSeries*>& series() const;
    void removeSeries(QCtmAbstractSeries* series);
    void removeSeries(int i);

protected:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
