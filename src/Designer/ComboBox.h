#pragma once

#include <QDesignerCustomWidgetInterface>

class ComboBox
    : public QObject
    , public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    ComboBox(QObject* parent = nullptr);
    bool isContainer() const override { return false; }
    bool isInitialized() const override { return m_initialized; }
    QIcon icon() const override { return {}; }
    QString domXml() const override;
    QString group() const override { return "Input Widgets"; }
    QString includeFile() const override { return "QCustomUi/QCtmComboBox.h"; }
    QString name() const override { return "QCtmComboBox"; }
    QString toolTip() const override { return "QCtmComboBox"; }
    QString whatsThis() const override { return "QCtmComboBox"; }
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
