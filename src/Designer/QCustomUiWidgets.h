#pragma once

#include <QDesignerCustomWidgetCollectionInterface>

#include <memory>

class QCustomUiWidgets
    : public QObject
    , public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QDesignerCustomWidgetCollectionInterface_iid)
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
public:
    QCustomUiWidgets(QObject* parent = 0);
    ~QCustomUiWidgets();
    QList<QDesignerCustomWidgetInterface*> customWidgets() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
