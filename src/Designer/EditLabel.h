#pragma once

#include <QDesignerCustomWidgetInterface>

class EditLabel
    : public QObject
    , public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    explicit EditLabel(QObject* parent = nullptr);

    bool isContainer() const override { return false; }
    bool isInitialized() const override { return m_initialized; }
    QIcon icon() const override { return QIcon(); }
    QString domXml() const override;
    QString group() const override { return "Display Widgets"; }
    QString includeFile() const override { return "QCustomUi/QCtmEditLabel.h"; }
    QString name() const override { return "QCtmEditLabel"; }
    QString toolTip() const override { return "QCtmEditLabel"; }
    QString whatsThis() const override { return "QCtmEditLabel"; }
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
