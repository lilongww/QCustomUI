#pragma once

#include "QCtmWindow.h"

#include <memory>
#include <QStackedWidget>

class QCtmTabWidget;
class QCtmNavigationSideBar;

namespace Ui
{
    class QCtmMainWindow;
}

class QCUSTOMUI_EXPORT QCtmMainWindow : public QCtmWindow
{
    Q_OBJECT

public:
    enum class SideBarArea
    {
        Left,
        Right
    };

    enum class MessageBarArea
    {
        Bottom,
        Right
    };

    QCtmMainWindow(QWidget *parent = nullptr);
    ~QCtmMainWindow();

    /**
     * @brief       添加主侧边栏，如果已存在主侧边栏，旧的将被销毁
     * @param[in]   mainSideBar 主侧边栏地址
     */
    void setNavigationSideBar(QCtmNavigationSideBar* mainSideBar);

    /**
     * @brief       获取主侧边栏地址，如果没有添加主侧边栏则返回空
     * @Return:     主侧边栏地址
     */
    QCtmNavigationSideBar* navigationSideBar()const;

    /**
     * @brief		移除侧边栏并放弃归属权
     */
    QCtmNavigationSideBar* takeNavigationSideBar();

    /**
     * @brief       移除主侧边栏
     */
    void removeNavigationSideBar();

    /**
     * @brief       添加次级侧边栏，如果次级侧边栏已存在，旧的将被销毁
     * @param[in]   subSideBar 次级侧边栏地址
     */
    void setSubSideBar(QWidget* subSideBar);

    /**
     * @brief       返回二级侧边栏地址
     * @Return:     二级侧边栏地址
     */
    QWidget* subSideBar()const;

    /**
     * @brief		移除二级侧边栏并放弃归属权
     */
    QWidget* takeSubSideBar();

    /**
     * @brief       移除次级侧边栏
     */
    void removeSubSideBar();

    /**
     * @brief       设置内容显示窗口
     * @param[in]   widget 窗口地址
     */
    void setContentWidget(QWidget* widget);

    /**
     * @brief       获取content窗口地址
     * @Return:     content窗口地址
     */
    QWidget* contentWidget()const;

    /**
     * @brief		移除content窗口并放弃归属权
     */
    QWidget* takeContentWidget();

    /**
     * @brief       设置消息栏，如已存在消息栏，旧的消息栏将被销毁
     * @param[in]   messageBar 消息栏地址
     */
    void setMessageBar(QCtmTabWidget* messageBar);

    /**
     * @brief       获取消息栏地址
     * @Return:     消息栏地址
     */
    QCtmTabWidget* messageBar()const;

    /**
     * @brief		移除消息栏并放弃归属权
     */
    QCtmTabWidget* takeMessageBar();

    /**
     * @brief       移除消息栏
     */
    void removeMessageBar();

    /**
     * @brief       设置属性栏，如果已存在属性栏，旧的属性栏将被销毁
     * @param[in]   propertyBar 属性栏地址
     */
    void setPropertyBar(QWidget* propertyBar);

    /**
     * @brief       获取属性栏地址
     * @Return:     属性栏地址
     */
    QWidget* propertyBar()const;

    /**
     * @brief		移除属性栏并放弃归属权
     */
    QWidget* takePropertyBar();

    /**
     * @brief       移除属性栏
     */
    void removePropertyBar();
    /**
     * @brief       设置侧边栏停靠位置
     * @param[in]   area 侧边栏显示区域，
     */
    void setSideBarArea(SideBarArea area);

    /**
     * @brief       获取侧边栏停靠位置
     * @Return:     侧边栏停靠位置
     */
    SideBarArea sideBarArea()const;

    /**
     * @brief		设置消息栏显示区域
     * @param[in]	area 区域
     */
    void setMessageBarArea(MessageBarArea area);

    /**
     * @brief		获取消息栏显示区域
     * @Return:		区域
     */
    MessageBarArea messageBarArea()const;

    /**
     * @brief       设置侧边栏的宽度
     * @param[in]   width 宽度
     */
    void setSubSideBarWidth(int width);

    /**
     * @brief       获取侧边栏的宽度
     * @Return:     宽度
     */
    int subSideBarWidth()const;

    /**
     * @brief       设置属性栏宽度
     * @param[in]   width 属性栏宽度
     */
    void setPropertyBarWidth(int width);

    /**
     * @brief       获取属性栏宽度
     * @Return:     宽度
     */
    int propertyBarWidth()const;

    /**
     * @brief       设置消息栏高度
     * @param[in]   height 高度
     */
    void setMessageBarHeight(int height);

    /**
     * @brief       获取消息栏高度
     * @Return:     高度
     */
    int messageBarHeight()const;
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void showEvent(QShowEvent*)override;
    void doSize();

    private slots:
    void onTabWidgetAreaButtonClicked();
private:
    QList<int>& swap(QList<int>& sizes);

private:
    struct Impl;
    std::shared_ptr<Impl> m_impl;

    Ui::QCtmMainWindow* ui;
};
