#pragma once

#include <QDesignerCustomWidgetInterface>

class CircleProgressBar
    : public QObject
    , public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    explicit CircleProgressBar(QObject* parent = nullptr);

    bool isContainer() const override { return false; }
    bool isInitialized() const override { return m_initialized; }
    QIcon icon() const override { return QIcon(); }
    QString domXml() const override;
    QString group() const override { return "Display Widgets"; }
    QString includeFile() const override { return "QCustomUi/QCtmCircleProgressBar.h"; }
    QString name() const override { return "QCtmCircleProgressBar"; }
    QString toolTip() const override { return "QCtmCircleProgressBar"; }
    QString whatsThis() const override { return "QCtmCircleProgressBar"; }
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
