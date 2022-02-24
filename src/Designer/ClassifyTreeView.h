#pragma once

#include <QDesignerCustomWidgetInterface>

class ClassifyTreeView
    : public QObject
    , public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    explicit ClassifyTreeView(QObject* parent = nullptr);

    bool isContainer() const override { return false; }
    bool isInitialized() const override { return m_initialized; }
    QIcon icon() const override { return QIcon(); }
    QString domXml() const override;
    QString group() const override { return "Item Views (Model-Based)"; }
    QString includeFile() const override { return "QCustomUi/QCtmClassifyTreeView.h"; }
    QString name() const override { return "QCtmClassifyTreeView"; }
    QString toolTip() const override { return "QCtmClassifyTreeView"; }
    QString whatsThis() const override { return "QCtmClassifyTreeView"; }
    QWidget* createWidget(QWidget* parent) override;
    void initialize(QDesignerFormEditorInterface* core) override
    {
        if (m_initialized)
            return;
        m_initialized = true;
    }

private:
    bool m_initialized { false };
};
