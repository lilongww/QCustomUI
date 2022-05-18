#pragma once

#include <QDesignerCustomWidgetInterface>

class SwitchButton
    : public QObject
    , public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    explicit SwitchButton(QObject* parent = nullptr);

    bool isContainer() const override { return false; }
    bool isInitialized() const override { return m_initialized; }
    QIcon icon() const override { return QIcon(); }
    QString domXml() const override;
    QString group() const override { return "Buttons"; }
    QString includeFile() const override { return "QCustomUi/QCtmSwitchButton.h"; }
    QString name() const override { return "QCtmSwitchButton"; }
    QString toolTip() const override { return "QCtmSwitchButton"; }
    QString whatsThis() const override { return "QCtmSwitchButton"; }
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
