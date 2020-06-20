#include "QCtmDrawerItemWidget.h"
#include "QCtmDrawerWidget.h"
#include "Private/QCtmDrawerItemTitle_p.h"

#include <QVBoxLayout>
#include <QScrollArea>

struct QCtmDrawerItemWidget::Impl
{
    QCtmDrawerItemTitle* title{ nullptr };
    QVBoxLayout* layout{ nullptr };
    QScrollArea* content{ nullptr };
    QWidget* widget{ nullptr };
    int size = 0;
    QCtmDrawerWidget* tree{ nullptr };
};

/*!
    \class      QCtmDrawerItemWidget
    \brief      QCtmDrawerItemWidget provide a item when add a widget to the QCtmDrawerWidget.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \fn         void QCtmDrawerItemWidget::expandChanged(bool expand)
    \brief      Emit this signal when the item's \a expand state is changed.
    \sa         QCtmDrawerItemWidget::titleClicked
*/

/*!
    \fn         void QCtmDrawerItemWidget::titleClicked(bool expand)
    \brief      Emit this signal when the title bar of the item has being clicked, \a expand.
    \sa         QCtmDrawerItemWidget::expandChanged
*/

/*!
    \brief      Constructs a item with the given \a title and \a parent.
*/
QCtmDrawerItemWidget::QCtmDrawerItemWidget(const QString& title, QCtmDrawerWidget* parent)
    : QWidget(parent)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->tree = parent;
    m_impl->layout = new QVBoxLayout(this);
    m_impl->layout->setMargin(0);
    m_impl->layout->setSpacing(0);

    m_impl->title = new QCtmDrawerItemTitle(this);
    m_impl->title->setText(title);
    m_impl->title->setObjectName("title");

    m_impl->layout->addWidget(m_impl->title, 0);
    m_impl->content = new QScrollArea(this);
    m_impl->content->setWidgetResizable(true);
    m_impl->content->setFrameStyle(QFrame::NoFrame);
    m_impl->content->hide();
    m_impl->layout->addWidget(m_impl->content, 1);
    m_impl->title->setFixedHeight(30);

    connect(m_impl->title, &QCtmDrawerItemTitle::clicked, this, &QCtmDrawerItemWidget::onClicked);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_impl->layout->setAlignment(m_impl->title, Qt::AlignTop);
    setExpand(false);
}

/*!
    \brief      Destroys the item.
*/
QCtmDrawerItemWidget::~QCtmDrawerItemWidget()
{

}

/*!
    \brief      Sets the given \a widget.
    \sa         widget()
*/
void QCtmDrawerItemWidget::setWidget(QWidget* widget)
{
    if (m_impl->widget)
        delete m_impl->widget;
    m_impl->widget = widget;
    m_impl->content->setWidget(widget);
    m_impl->title->update();
    if (!m_impl->widget)
        setExpand(false);
}

/*!
    \brief      Returns the widget.
    \sa         setWidget
*/
QWidget* QCtmDrawerItemWidget::widget() const
{
    return m_impl->widget;
}

/*!
    \brief      Sets the \a title.
    \sa         title()
*/
void QCtmDrawerItemWidget::setTitle(const QString& title)
{
    m_impl->title->setText(title);
}

/*!
    \brief      Returns the title.
    \sa         setTitle
*/
QString QCtmDrawerItemWidget::title() const
{
    return m_impl->title->text();
}

/*!
    \brief      Returns expand state of the item.
    \sa         setExpand
*/
bool QCtmDrawerItemWidget::isExpand() const
{
    return m_impl->title->isExpand();
}

/*!
    \brief      Sets the \a expand state of the item.
    \sa         isExpand
*/
void QCtmDrawerItemWidget::setExpand(bool expand)
{
    if (isExpand() == expand)
        return;
    if (!m_impl->widget)
        return;
    m_impl->title->setExpand(expand);
    m_impl->content->setVisible(expand);
    if (m_impl->tree)
    {
        emit this->expandChanged(expand);
        this->metaObject()->invokeMethod(m_impl->tree
            , "childExpandChanged"
            , Qt::QueuedConnection
            , Q_ARG(QCtmDrawerItemWidget*, this)
            , Q_ARG(bool, expand));
    }
    if (!expand)
        m_impl->size = this->height();
}

/*!
    \brief      Changes the expand state when the title bar clicked.
*/
void QCtmDrawerItemWidget::onClicked()
{
    emit(titleClicked(!isExpand()));
    if (m_impl->widget)
        setExpand(!isExpand());
}

/*!
    \reimp
*/
void QCtmDrawerItemWidget::resizeEvent([[maybe_unused]] QResizeEvent* event)
{
    if (!isExpand())
    {
        if (m_impl->tree)
        {
            metaObject()->invokeMethod(m_impl->tree, "doResize", Qt::QueuedConnection);
        }
    }
}

/*!
    \brief      Returns a suggest size.
    \sa         setSuggestSize
*/
int QCtmDrawerItemWidget::suggestSize() const
{
    return m_impl->size < this->minimumSizeHint().height() ? this->minimumSizeHint().height() : m_impl->size;
}

/*!
    \brief      Sets a suggest \a size.
    \sa         suggestSize()
*/
void QCtmDrawerItemWidget::setSuggestSize(int size)
{
    m_impl->size = size;
}

/*!
    \overload   addAction
                Creates a action with the given \a text.
                And adds and returns the action to the title bar.
    \sa         QCtmDrawerItemWidget::addAction, removeAction
*/
QAction* QCtmDrawerItemWidget::addAction(const QString& text)
{
    return insertAction(count(), text);
}

/*!
    \brief      Creates a action with the given \a icon and \a text.
                And adds and returns the action to the title bar.
    \sa         insertAction, removeAction
*/
QAction* QCtmDrawerItemWidget::addAction(const QIcon& icon, const QString& text)
{
    return insertAction(count(), icon, text);
}

/*!
    \brief      Insert the given \a action to the position of \a index.
    \sa         addAction, removeAction
*/
void QCtmDrawerItemWidget::insertAction(int index, QAction* action)
{
    m_impl->title->insertAction(index, action);
}

/*!
    \overload   insertAction
                Creates a action with the given \a text.
                And inserts and returns the action to the title bar with \a index.
    \sa         QCtmDrawerItemWidget::insertAction, removeAction
*/
QAction* QCtmDrawerItemWidget::insertAction(int index, const QString& text)
{
    return insertAction(index, QIcon(), text);
}

/*!
    \overload   insertAction
                Creates a action with the given \a icon and \a text.
                And inserts and returns the action to the title bar with \a index.
    \sa         QCtmDrawerItemWidget::insertAction, removeAction
*/
QAction* QCtmDrawerItemWidget::insertAction(int index, const QIcon& icon, const QString& text)
{
    auto action = new QAction(icon, text, m_impl->title);
    insertAction(index, action);
    return action;
}

/*!
    \brief      Remove the given \a action.
    \sa         addAction, insertAction
*/
void QCtmDrawerItemWidget::removeAction(QAction* action)
{
    m_impl->title->removeAction(action);
}

/*!
    \brief      Returns the count of actions.
*/
int QCtmDrawerItemWidget::count() const
{
    return m_impl->title->actions().size();
}

/*!
    \brief      Returns the index of the given \a action.
    \sa         actionAt
*/
int QCtmDrawerItemWidget::indexOf(QAction* action)
{
    return m_impl->title->indexOf(action);
}

/*!
    \brief      Returns the action of the given \a index.
    \sa         indexOf
*/
QAction* QCtmDrawerItemWidget::actionAt(int index)
{
    return m_impl->title->actionAt(index);
}
