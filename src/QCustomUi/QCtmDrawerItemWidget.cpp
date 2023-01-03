/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2023 LiLong                                              **
**  This file is part of QCustomUi.                                             **
**                                                                              **
**  QCustomUi is free software: you can redistribute it and/or modify           **
**  it under the terms of the GNU Lesser General Public License as published by **
**  the Free Software Foundation, either version 3 of the License, or           **
**  (at your option) any later version.                                         **
**                                                                              **
**  QCustomUi is distributed in the hope that it will be useful,                **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of              **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               **
**  GNU Lesser General Public License for more details.                         **
**                                                                              **
**  You should have received a copy of the GNU Lesser General Public License    **
**  along with QCustomUi.  If not, see <https://www.gnu.org/licenses/>.         **
**********************************************************************************/
#include "QCtmDrawerItemWidget.h"
#include "Private/QCtmDrawerItemTitle_p.h"
#include "QCtmDrawerWidget.h"

#include <QScrollArea>
#include <QVBoxLayout>

struct QCtmDrawerItemWidget::Impl
{
    QCtmDrawerItemTitle* title { nullptr };
    QVBoxLayout* layout { nullptr };
    QScrollArea* content { nullptr };
    QWidget* widget { nullptr };
    int size = 0;
    QCtmDrawerWidget* tree { nullptr };
};

/*!
    \class      QCtmDrawerItemWidget
    \brief      抽屉窗口 QCtmDrawerWidget 的抽屉项.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmDrawerItemWidget.h

    \b          {抽屉窗口的截图，抽屉项为其中一栏:}
    \image      QCtmDrawerWidgetDetail.png
*/

/*!
    \fn         void QCtmDrawerItemWidget::expandChanged(bool expand)
    \brief      当抽屉项的扩展状态发生变化时，发送该信号 \a expand.
    \sa         QCtmDrawerItemWidget::titleClicked
*/

/*!
    \fn         void QCtmDrawerItemWidget::titleClicked(bool expand)
    \brief      当抽屉项的标题栏被点击时发送该信号 \a expand.
    \sa         QCtmDrawerItemWidget::expandChanged
*/

/*!
    \fn         void QCtmDrawerItemWidget::iconSizeChanged(const QSize& size);
    \brief      当Action的图标大小发生改变时发送该信号 \a size.
    \sa         QCtmDrawerItemWidget::setIconSize
*/

/*!
    \brief      构造一个标题为 \a title, 父窗口为 \a parent 的抽屉窗口.
*/
QCtmDrawerItemWidget::QCtmDrawerItemWidget(const QString& title, QCtmDrawerWidget* parent)
    : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->tree   = parent;
    m_impl->layout = new QVBoxLayout(this);
    m_impl->layout->setContentsMargins(0, 0, 0, 0);
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
    \brief      销毁该抽屉项对象.
*/
QCtmDrawerItemWidget::~QCtmDrawerItemWidget() {}

/*!
    \brief      设置抽屉项容纳的部件 \a widget.
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
    \brief      返回该抽屉项容纳的部件.
    \sa         setWidget
*/
QWidget* QCtmDrawerItemWidget::widget() const { return m_impl->widget; }

/*!
    \brief      设置抽屉项的标题 \a title.
    \sa         title()
*/
void QCtmDrawerItemWidget::setTitle(const QString& title) { m_impl->title->setText(title); }

/*!
    \brief      返回抽屉项的标题.
    \sa         setTitle
*/
QString QCtmDrawerItemWidget::title() const { return m_impl->title->text(); }

/*!
    \brief      返回该抽屉项的展开状态.
    \sa         setExpand
*/
bool QCtmDrawerItemWidget::isExpand() const { return m_impl->title->isExpand(); }

/*!
    \brief      设置该抽屉项的展开状态 \a expand.
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
        this->metaObject()->invokeMethod(
            m_impl->tree, "childExpandChanged", Qt::QueuedConnection, Q_ARG(QCtmDrawerItemWidget*, this), Q_ARG(bool, expand));
    }
    if (!expand)
        m_impl->size = this->height();
}

/*!
    \brief      响应标题栏点击信号.
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
    \brief      返回抽屉项的建议大小.
    \sa         setSuggestSize
*/
int QCtmDrawerItemWidget::suggestSize() const
{
    return m_impl->size < this->minimumSizeHint().height() ? this->minimumSizeHint().height() : m_impl->size;
}

/*!
    \brief      设置抽屉项的建议大小 \a size.
    \sa         suggestSize()
*/
void QCtmDrawerItemWidget::setSuggestSize(int size) { m_impl->size = size; }

/*!
    \overload   addAction
                在标题栏上添加一个文本为 \a text 的Action.
    \sa         QCtmDrawerItemWidget::addAction, removeAction
*/
QAction* QCtmDrawerItemWidget::addAction(const QString& text) { return insertAction(count(), text); }

/*!
    \brief      在标题栏上添加一个图标为 \a icon 文本为 \a text 的Action.
    \sa         insertAction, removeAction
*/
QAction* QCtmDrawerItemWidget::addAction(const QIcon& icon, const QString& text) { return insertAction(count(), icon, text); }

/*!
    \overload   addAction
                在标题栏上添加一个图标为 \a icon 的Action.
    \sa         insertAction, removeAction
*/
QAction* QCtmDrawerItemWidget::addAction(const QIcon& icon) { return addAction(icon, QString()); }

/*!
    \brief      在标题栏上的 \a index 位置插入一个 \a action.
    \sa         addAction, removeAction
*/
void QCtmDrawerItemWidget::insertAction(int index, QAction* action) { m_impl->title->insertAction(index, action); }

/*!
    \overload   insertAction
                在标题栏上的 \a index 位置插入一个文本为 \a text 的Action.
    \sa         QCtmDrawerItemWidget::insertAction, removeAction
*/
QAction* QCtmDrawerItemWidget::insertAction(int index, const QString& text) { return insertAction(index, QIcon(), text); }

/*!
    \overload   insertAction
                在标题栏上的 \a index 位置插入一个图标为 \a icon 文本为 \a text 的Action.
    \sa         QCtmDrawerItemWidget::insertAction, removeAction
*/
QAction* QCtmDrawerItemWidget::insertAction(int index, const QIcon& icon, const QString& text)
{
    auto action = new QAction(icon, text, m_impl->title);
    insertAction(index, action);
    return action;
}

/*!
    \brief      移除给予的 \a action.
    \sa         addAction, insertAction
*/
void QCtmDrawerItemWidget::removeAction(QAction* action) { m_impl->title->removeAction(action); }

/*!
    \brief      返回Action的数量.
*/
int QCtmDrawerItemWidget::count() const { return m_impl->title->actions().size(); }

/*!
    \brief      返回 \a action 的位置.
    \sa         actionAt
*/
int QCtmDrawerItemWidget::indexOf(QAction* action) { return m_impl->title->indexOf(action); }

/*!
    \brief      返回 \a index 位置的Action.
    \sa         indexOf
*/
QAction* QCtmDrawerItemWidget::actionAt(int index) { return m_impl->title->actionAt(index); }

/*!
    \brief      设置Action的大小 \a size.
    \sa         iconSize
*/
void QCtmDrawerItemWidget::setIconSize(const QSize& size)
{
    m_impl->title->setIconSize(size);
    emit iconSizeChanged(size);
}

/*!
    \brief      返回Action的大小.
    \sa         setIconSize
*/
const QSize& QCtmDrawerItemWidget::iconSize() const { return m_impl->title->iconSize(); }
