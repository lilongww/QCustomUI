#pragma once

#include <QDesignerCustomWidgetInterface>

class MultiComboBox
    : public QObject
    , public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    explicit MultiComboBox(QObject* parent = nullptr);

    bool isContainer() const override { return false; }
    bool isInitialized() const override { return m_initialized; }
    QIcon icon() const override;
    QString domXml() const override;
    QString group() const override { return "Input Widgets"; }
    QString includeFile() const override { return "QCustomUi/QCtmMultiComboBox.h"; }
    QString name() const override { return "QCtmMultiComboBox"; }
    QString toolTip() const override { return "QCtmMultiComboBox"; }
    QString whatsThis() const override { return "QCtmMultiComboBox"; }
    QWidget* createWidget(QWidget* parent) override;
    void initialize(QDesignerFormEditorInterface* core) override;

private:
    bool m_initialized { false };
};
