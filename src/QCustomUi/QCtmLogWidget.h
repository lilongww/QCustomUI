#pragma once

#include "qcustomui_global.h"
#include "QCtmLogEvent.h"

#include <QWidget>
#include <QIcon >

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

    /**
     * @brief		设置日志插入顺序
     * @param[in]	mode 日志插入顺序
     */
    void setLogInsertMode(LogInsertMode mode);

    /**
     * @brief		获取日志插入顺序
     * @Return:		日志插入顺序
     */
    LogInsertMode logInsertMode() const;

    /**
     * @brief		设置日志列是否显示
     * @param[in]	column 列
     * @param[in]	visible 是否显示
     */
    void setColumnVisible(QCtmLogColumn column, bool visible);

    /**
     * @brief		获取日志列是否显示
     * @param[in]	column 列
     * @Return:		是否显示
     */
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
