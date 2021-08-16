#include "QCtmInputDialog.h"
#include "QCtmTitleBar.h"
#ifdef Q_OS_WIN
#include "Private/QCtmFramelessDelegate_win.h"
#else
#include "Private/QCtmFramelessDelegate_p.h"
#endif

#include <QVBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QApplication>
#include <QStyle>
#include "private/qwidget_p.h"

struct QCtmInputDialog::Impl
{
    QCtmTitleBar* title{ nullptr };
#ifdef Q_OS_WIN
    QCtmWinFramelessDelegate* delegate{ nullptr };
#else
    QCtmFramelessDelegate* delegate{ nullptr };
#endif
    QVBoxLayout* layout{ nullptr };
};

/*!
    \class      QCtmInputDialog
    \brief      功能与QInputDialog相同，实现了QCustomUi的窗口风格.
    \inherits   QInputDialog
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmInputDialog.h
*/

/*!
    \brief      构造函数 \a parent, \a flags.
*/
QCtmInputDialog::QCtmInputDialog(QWidget* parent, Qt::WindowFlags flags)
    : QInputDialog(parent, flags)
    , m_impl(std::make_unique<Impl>())
{
    sizeHint();
    m_impl->layout = new QVBoxLayout;
    m_impl->layout->setMargin(0);
    {
        auto layout = qobject_cast<QVBoxLayout*>(this->layout());
        auto w = new QWidget(this);
        w->setLayout(layout);
        layout->setParent(w);
        auto style = this->style();
        layout->setContentsMargins(style->pixelMetric(QStyle::PM_LayoutLeftMargin)
            , style->pixelMetric(QStyle::PM_LayoutTopMargin)
            , style->pixelMetric(QStyle::PM_LayoutRightMargin)
            , style->pixelMetric(QStyle::PM_LayoutBottomMargin));
        m_impl->layout->addWidget(w);
    }
    setLayout(m_impl->layout);
    m_impl->title = new QCtmTitleBar(this);
    m_impl->title->setObjectName("ctmDialogTitleBar");
    m_impl->layout->insertWidget(0, m_impl->title);
#ifdef Q_OS_WIN
    m_impl->delegate = new QCtmWinFramelessDelegate(this, m_impl->title);
#else
    m_impl->delegate = new QCtmFramelessDelegate(this, m_impl->title);
    setWindowFlag(Qt::Dialog);
#endif
    setWindowFlag(Qt::WindowMinMaxButtonsHint, false);
}

/*!
    \brief      析构函数.
*/
QCtmInputDialog::~QCtmInputDialog()
{

}

/*!
    \brief      设置标题栏 \a titleBar.
    \sa         titleBar, removeTitleBar
*/
void QCtmInputDialog::setTitleBar(QCtmTitleBar* titleBar)
{
    if (m_impl->title)
    {
        m_impl->layout->replaceWidget(m_impl->title, titleBar);
        delete m_impl->title;
    }
    else
    {
        m_impl->layout->insertWidget(0, titleBar);
    }
    m_impl->delegate->addMoveBar(titleBar);
    m_impl->title = titleBar;
}

/*!
    \brief      返回标题栏.
    \sa         setTitleBar, removeTitleBar
*/
QCtmTitleBar* QCtmInputDialog::titleBar() const
{
    return m_impl->title;
}

/*!
    \brief      移除标题栏.
    \sa         setTitleBar, titleBar
*/
void QCtmInputDialog::removeTitleBar()
{
    if (m_impl->title)
    {
        delete m_impl->title;
        m_impl->title = nullptr;
    }
}

/*!
    \overload
*/
QString QCtmInputDialog::getText(QWidget* parent, const QString& title, const QString& label, QLineEdit::EchoMode echo, const QString& text, bool* ok, Qt::WindowFlags flags, Qt::InputMethodHints inputMethodHints)
{
    QCtmInputDialog dlg(parent, flags);
    dlg.setWindowTitle(title);
    dlg.setLabelText(label);
    dlg.setTextValue(text);
    dlg.setTextEchoMode(echo);
    dlg.setInputMethodHints(inputMethodHints);

    const int ret = dlg.exec();
    if (ok)
        *ok = !!ret;
    if (ret) {
        return dlg.textValue();
    }
    else {
        return QString();
    }
}

/*!
    \overload
*/
QString QCtmInputDialog::getMultiLineText(QWidget* parent, const QString& title, const QString& label, const QString& text, bool* ok, Qt::WindowFlags flags, Qt::InputMethodHints inputMethodHints)
{
    QCtmInputDialog dialog(parent, flags);
    dialog.setOptions(QInputDialog::UsePlainTextEditForTextInput);
    dialog.setWindowTitle(title);
    dialog.setLabelText(label);
    dialog.setTextValue(text);
    dialog.setInputMethodHints(inputMethodHints);

    const int ret = dialog.exec();
    if (ok)
        *ok = !!ret;
    if (ret) {
        return dialog.textValue();
    }
    else {
        return QString();
    }
}

/*!
    \overload
*/
QString QCtmInputDialog::getItem(QWidget* parent, const QString& title, const QString& label, const QStringList& items, int current, bool editable, bool* ok, Qt::WindowFlags flags, Qt::InputMethodHints inputMethodHints)
{
    QString text(items.value(current));

    QCtmInputDialog dialog(parent, flags);
    dialog.setWindowTitle(title);
    dialog.setLabelText(label);
    dialog.setComboBoxItems(items);
    dialog.setTextValue(text);
    dialog.setComboBoxEditable(editable);
    dialog.setInputMethodHints(inputMethodHints);

    const int ret = dialog.exec();
    if (ok)
        *ok = !!ret;
    if (ret) {
        return dialog.textValue();
    }
    else {
        return text;
    }
}

/*!
    \overload
*/
int QCtmInputDialog::getInt(QWidget* parent, const QString& title, const QString& label, int value, int minValue, int maxValue, int step, bool* ok, Qt::WindowFlags flags)
{
    QCtmInputDialog dialog(parent, flags);
    dialog.setWindowTitle(title);
    dialog.setLabelText(label);
    dialog.setIntRange(minValue, maxValue);
    dialog.setIntValue(value);
    dialog.setIntStep(step);

    const int ret = dialog.exec();
    if (ok)
        *ok = !!ret;
    if (ret) {
        return dialog.intValue();
    }
    else {
        return value;
    }
}

/*!
    \overload
*/
double QCtmInputDialog::getDouble(QWidget* parent
    , const QString& title
    , const QString& label
    , double value
    , double minValue
    , double maxValue
    , int decimals
    , bool* ok
    , Qt::WindowFlags flags
    , double step)
{
    QCtmInputDialog dialog(parent, flags);
    dialog.setWindowTitle(title);
    dialog.setLabelText(label);
    dialog.setDoubleDecimals(decimals);
    dialog.setDoubleRange(minValue, maxValue);
    dialog.setDoubleValue(value);
    dialog.setDoubleStep(step);

    const int ret = dialog.exec();
    if (ok)
        *ok = !!ret;
    if (ret)
        return dialog.doubleValue();
    else
        return value;
}

/*!
    \reimp
*/
void QCtmInputDialog::hideEvent(QHideEvent*)
{
    auto closeBtn = m_impl->title->findChild<QWidget*>("closeBtn");
    if (closeBtn)
    {
        auto e = new QEvent(QEvent::Type::Leave);
        qApp->sendEvent(closeBtn, e);
    }
}

/*!
    \reimp
*/
bool QCtmInputDialog::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
#ifdef Q_OS_WIN
    if (!m_impl->delegate)
        return QInputDialog::nativeEvent(eventType, message, result);
    if (!m_impl->delegate->nativeEvent(eventType, message, result))
        return QInputDialog::nativeEvent(eventType, message, result);
    else
        return true;
#else
    return QInputDialog::nativeEvent(eventType, message, result);
#endif
}
