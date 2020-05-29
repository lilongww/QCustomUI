#pragma once

#include "qcustomui_global.h"
#include "QCtmLogEvent.h"

#include <QWidget>
#include <QIcon>

#include <memory>

class QCUSTOMUI_EXPORT QCtmLogWidget : public QWidget
{
    Q_OBJECT
        Q_PROPERTY(QIcon infoIcon READ infoIcon WRITE setInfoIcon)
        Q_PROPERTY(QIcon warningIcon READ warningIcon WRITE setWarningIcon)
        Q_PROPERTY(QIcon errorIcon READ errorIcon WRITE setErrorIcon)
public:

    enum class QCtmLogColumn    //日志列枚举
    {
        Level,                  //日志等级
        Time,                   //日志时间
        Description             //日志内容
    };

    QCtmLogWidget(const QString& objectName, QWidget *parent = Q_NULLPTR);
    ~QCtmLogWidget();

    void setLogInsertMode(LogInsertMode mode);
    LogInsertMode logInsertMode() const;
    void setColumnVisible(QCtmLogColumn column, bool visible);
    bool columnVisible(QCtmLogColumn column) const;
    void setInfoIcon(const QIcon& icon);
    const QIcon& infoIcon() const;
    void setWarningIcon(const QIcon& icon);
    const QIcon& warningIcon() const;
    void setErrorIcon(const QIcon& icon);
    const QIcon& errorIcon() const;
    void setMaximumCount(int count);
    int maximumCount() const;
private:
    void init();
    void changeEvent(QEvent* e) override;
private slots:
    void updateLogCount();
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
