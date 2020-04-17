#pragma once

#include <QIcon>
#include <QAction>

#include <memory>

class QCtmTabWidget;
class QCtmNavigationSideBar;
class QCtmNavigationBar;
class QCtmMainWindow;
class QCtmNavigationMainMenu;
class QCtmActivityInterface;
class QCtmAbstractMessage;
class QCtmNavigationSidePanel;
class SWContextInterface;
typedef std::shared_ptr<QCtmAbstractMessage> QCtmAbstractMessagePtr;

typedef int ActionPosition;

constexpr auto SWPluginKey = "QCustomUi.SwitchLauncher.PluginInterface";

struct ActionInfo
{
    ActionInfo(QAction* a, ActionPosition p, QCtmNavigationSidePanel* sp = nullptr, bool always = false)
        : action(a)
        , position(p)
        , panel(sp)
        , alwaysShow(always) {}
    QAction* action{ nullptr };		            //Action地址
	QCtmNavigationSidePanel* panel{ nullptr };  //弹出面板地址，如果panel不为空，则action将会与panel绑定
    ActionPosition position;		            //Action的停靠位置
    bool alwaysShow{ false };		            //Action是否总是显示
};

struct TabPageInfo
{
    QString title;					            //Tab页的标题
    QWidget* widget{ nullptr };		            //Tab页的内容区
    QList<QAction*> actions;		            //Tab页右上角的Action
};

struct StatusBarInfo
{
    enum WidgetPosition
    {
        Left,
        Right
    };
    QWidget* widget{ nullptr };		            //要放在状态栏上的weiget
    int stretch{ 0 };				            //窗口所占比例
    WidgetPosition position{ Left };            //窗口停靠方向
};

struct MenuInfo
{
    QMenu* menu{ nullptr };			            //要放在菜单栏上的子菜单
    bool alwaysShow{ false };		            //是否总是显示
};

struct SWUserData
{
    QVariant data;					            //用于手动切换插件时传输附加信息
};

enum SWMessageEventType
{
	Add,
	Delete,
	Clicked
};


enum SWPluginAttribute
{
	SWPA_None,
	SWPA_DontShowInPluginMenu = 0x01,                  //不显示在插件菜单中
	SWPA_DontShowPluginNameInTitle = 0x02              //不显示插件名称到标题栏
};

Q_DECLARE_FLAGS(SWPluginAttributes, SWPluginAttribute)
Q_DECLARE_OPERATORS_FOR_FLAGS(SWPluginAttributes)

enum class SWContextAttribute
{
	SWCA_None,
	SWCA_DontShowTitle = 0x01,                           //不显示标题栏
	SWCA_LogInsertDESC = 0x02,                           //日志倒序插入
	SWCA_QuitAlert = 0x04								 //主窗口退出时提示
};

Q_DECLARE_FLAGS(SWContextAttributes, SWContextAttribute)
Q_DECLARE_OPERATORS_FOR_FLAGS(SWContextAttributes)

//插件接口
class SWPluginInterface
{
public:
    SWPluginInterface() = default;
    virtual ~SWPluginInterface() = default;

	/**
	 * @brief		插件id，数值低的越先被加载和显示
	 */
    virtual int id() const = 0;

	/**
	 * @brief		插件的图标，将显示到QCtmNavigationMainMenu上
	 */
    virtual QIcon icon() const = 0;

	/**
	 * @brief		插件的名称，将显示到QCtmNavigationMainMenu，并在插件切换时显示到标题栏
	 */
    virtual QString name() const = 0;

	/**
	 * @brief		插件初始化，插件被加载后会先调用初始化函数
	 * @param[in]	context 框架上下文，用于框架设置
	 */
    virtual bool initialize(SWContextInterface* context) = 0;

	/**
	 * @brief		插件被卸载前调用该函数
	 */
    virtual void release() = 0;

	/**
	 * @brief		插件的样式表由此接口返回给框架
	 * @param[in]	theme 样式主题
	 */
    virtual QString styleSheet(const QString& theme) const = 0;

    /**
     * @brief		切换插件时调用该接口
     * @param[in]	current true切入当前插件，false切出当前插件
     * @Return:		true:当前插件可以切换，false:当前插件拒绝切换
     */
	virtual bool currentPluginChangeEvent(bool current, const SWPluginInterface* old, const SWUserData& data = SWUserData()) { return true; }

	/**
	 * @brief		窗口内容，没有则返回空
	 */
    virtual QWidget* content() const = 0;

	/**
	 * @brief		侧边栏对象，没有侧边栏则返回空
	 */
	virtual QCtmNavigationSideBar* sideBar() const { return nullptr; }

	/**
	 * @brief		次级侧边栏对象，没有则返回空
	 */
	virtual QWidget* subSideBar() const { return nullptr; }

	/**
	 * @brief		要在QCtmNavigationBar上显示的Action，没有则返回空list
	 */
	virtual QList<ActionInfo> navigationBarActions() const { return QList<ActionInfo>(); }

	/**
	 * @brief		要在消息栏上显示的Tab页，没有则返回空list
	 */
	virtual QList<TabPageInfo> messageBarTabPages() const { return QList<TabPageInfo>(); }

	/**
	 * @brief		要在状态栏上显示的窗口
	 */
	virtual QList<StatusBarInfo> statusBarWidgets() const { return QList<StatusBarInfo>(); }

	/**
	 * @brief		要在标题栏的菜单栏上显示的菜单项
	 */
	virtual QList<MenuInfo> subMenus() const { return QList<MenuInfo>(); }

	/**
	 * @brief		所有的插件加载完成后调用该函数
	 */
	virtual void allActivitiesInitialized() {};

    /**
     * @brief		插件属性设置
     * @Return:		插件属性集合
     */
    virtual SWPluginAttributes attributes() const { return SWPluginAttribute::SWPA_None; }
};

//插件消息槽
class SWPluginSlots
{
public:
	SWPluginSlots() = default;
	~SWPluginSlots() = default;
	/**
	 * @brief		菜单栏action点击事件
	 * @param[in]	action 被点击的action
	 */
	virtual void onMenuBarActionTriggered(QAction* action) {}

	/**
	 * @brief		响应消息提示栏
	 * @param[in]	msg 消息内容
	 * @param[in]	type 消息操作，增/删/点击
	 * @Return:		
	 */
	virtual void onMessageTip(QCtmAbstractMessagePtr msg, SWMessageEventType type) {}

	/**
	 * @brief		响应用户按钮点击事件
	 * @Return:		
	 */
	virtual void onUserActionTriggered() {}
};

Q_DECLARE_INTERFACE(SWPluginInterface, SWPluginKey)

//插件上下文
class SWContextInterface
{
public:
	SWContextInterface() = default;
	~SWContextInterface() = default;

	/**
	 * @brief		根据插件名称获取插件地址
	 */
	virtual SWPluginInterface* plugin(const QString& name) const = 0;

	/**
	 * @brief		获取插件地址列表
	 */
	virtual QList<SWPluginInterface*> plugins() const = 0;

	/**
	 * @brief		获取插件名称列表
	 */
	virtual QStringList activitiesNames() const = 0;

	/**
	 * @brief		设置当前显示插件
	 * @param[in]	activity 当前插件地址
	 * @param[in]	data 附加数据
	 * @Return:		是否切换成功
	 */
	virtual bool setCurrentPlugin(SWPluginInterface* activity, const SWUserData& data = SWUserData()) = 0;

    /**
     * @brief		当前显示的插件地址
     * @Return:		插件地址
     */
    virtual SWPluginInterface* currentPlugin() const = 0;

	/**
	 * @brief		向消息提示框添加一条消息
	 * @param[in]	msg 消息内容
	 */
	virtual void addMessageTip(QCtmAbstractMessagePtr msg) = 0;

	/**
	 * @brief		添加Logo
	 * @param[in]	icon logo图标
	 * @param[in]	pos logo停靠方向
	 * @Return:		logo地址
	 */
	virtual QAction* addLogo(const QIcon& icon, ActionPosition pos = 0) = 0;

	/**
	 * @brief		设置用户名和权限
	 * @param[in]	user 用户名
	 * @param[in]	authority 权限
	 */
	virtual void setUserInfo(const QString& user, const QString& authority) = 0;

    /**
     * @brief		设置消息栏是否隐藏
     * @param[in]	visible true:显示，false:隐藏
     */
    virtual void setMessageBarVisible(bool visible) = 0;

    /**
     * @brief		获取消息栏是否隐藏
     */
    virtual bool messageBarVisible() const = 0;

    /**
     * @brief		设置语言，目前支持CN和EN两种，即中文和英文，默认为英文。
     *              目前未实现动态语言切换，现在以修改配置文件为准。
     * @param[in]	language 语言
     */
    virtual void setLanguage(const QString& language) = 0;

    /**
     * @brief		获取当前语言
     * @Return:		当前语言
     */
    virtual const QString& language() const = 0;

    /**
     * @brief		设置Context特性
     * @param[in]	features 特性集合
     */
    virtual void setAttributes(SWContextAttributes features) = 0;

	/**
	 * @brief		设置Context特性
	 * @param[in]	attribute 属性
	 * @param[in]	enable 启用/禁用
	 */
	virtual void setAttribute(SWContextAttribute attribute, bool enable = true) = 0;

    /**
     * @brief		获取context的特性
     * @Return:		特性集合
     */
    virtual SWContextAttributes attributes() const = 0;

	/**
	 * @brief		设置消息栏当前显示页面
	 * @param[in]	index 页面索引
	 */
	virtual void setMessageBarCurrentIndex(int index) = 0;

	/**
	 * @brief		获取消息栏当前显示页面
	 * @param[in]	
	 * @param[in]	
	 * @Return:		
	 */
	virtual int messageBarCurrentIndex() const = 0;

    /**
     * @brief		获取默认样式
     * @param[in]	theme 样式名称
     * @Return:		样式源码
     */
    virtual QString defaultStyleSheet(const QString& theme) const = 0;

	/**
	 * @brief		获取主窗口地址
	 */
	virtual QCtmMainWindow* mainWindow() const = 0;
};
