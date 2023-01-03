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

#include "QCtmEditLabel.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPainter>
#include <QToolButton>

constexpr int EditButtonSize  = 16;
constexpr int EditButtonSpace = 4;
constexpr int extentedWidth() { return (EditButtonSpace + EditButtonSize) * 2; }

struct QCtmEditLabel::Impl
{
    bool readOnly { false };
    QToolButton* editButton { nullptr };
    QIcon finishButtonIcon { ":/QCustomUi/Resources/ok.png" };
    QIcon revertButtonIcon { ":/QCustomUi/Resources/cancel.png" };
};

/*!
    \class      QCtmEditLabel
    \brief      QLabel的扩展，可进行编辑.
    \inherits   QLabel
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmEditLabel.h

    \b          {截图:}
    \image      QCtmEditLabelDetail.png
*/

/*!
    \property   QCtmEditLabel::readOnly
    \brief      是否只读.
*/

/*!
    \property   QCtmEditLabel::editButtonIcon
    \brief      编辑按钮的图标.
*/

/*!
    \fn         void QCtmEditLabel::editFinished();
    \brief      编辑完成时发送该信号.
*/

/*!
    \brief      构造函数 \a parent, \a f.
*/
QCtmEditLabel::QCtmEditLabel(QWidget* parent, Qt::WindowFlags f) : QLabel(parent, f), m_impl(std::make_unique<Impl>()) { init(); }

/*!
    \brief      构造函数，并设置显示文字 \a text, \a parent, \a f.
*/
QCtmEditLabel::QCtmEditLabel(const QString& text, QWidget* parent /*= nullptr*/, Qt::WindowFlags f) : QCtmEditLabel(parent, f)
{
    setText(text);
}

/*!
    \brief      析构函数.
*/
QCtmEditLabel::~QCtmEditLabel() {}

/*!
    \brief      设置是否只读 \a flag.
    \sa         isReadOnly
*/
void QCtmEditLabel::setReadOnly(bool flag) { m_impl->readOnly = flag; }

/*!
    \brief      返回是否只读.
    \sa         setReadOnly
*/
bool QCtmEditLabel::isReadOnly() const { return m_impl->readOnly; }

/*!
    \brief      设置编辑按钮图标 \a icon.
    \sa         editButtonIcon
*/
void QCtmEditLabel::setEditButtonIcon(const QIcon& icon) { m_impl->editButton->setIcon(icon); }

/*!
    \brief      返回编辑按钮图标.
*/
QIcon QCtmEditLabel::editButtonIcon() const { return m_impl->editButton->icon(); }

/*!
    \brief      设置编辑完成按钮图标 \a icon.
    \sa         finishButtonIcon()
*/
void QCtmEditLabel::setFinishButtonIcon(const QIcon& icon) { m_impl->finishButtonIcon = icon; }

/*!
    \brief      返回编辑完成按钮图标.
    \sa         setFinishButtonIcon
*/
QIcon QCtmEditLabel::finishButtonIcon() const { return m_impl->finishButtonIcon; }

/*!
    \brief      设置回退按钮图标 \a icon.
    \sa         revertButtonIcon()
*/
void QCtmEditLabel::setRevertButtonIcon(const QIcon& icon) { m_impl->revertButtonIcon = icon; }

/*!
    \brief      返回回退按钮图标.
    \sa         setRevertButtonIcon
*/
QIcon QCtmEditLabel::revertButtonIcon() const { return m_impl->revertButtonIcon; }

/*!
    \reimp
*/
bool QCtmEditLabel::event(QEvent* e)
{
    switch (e->type())
    {
    case QEvent::HoverEnter:
        if (!m_impl->readOnly)
            m_impl->editButton->setVisible(true);
        break;
    case QEvent::HoverLeave:
        if (!m_impl->readOnly || m_impl->editButton->isVisible())
            m_impl->editButton->setVisible(false);
        break;
    default:
        break;
    }
    return QLabel::event(e);
}

/*!
    \reimp
*/
QSize QCtmEditLabel::minimumSizeHint() const
{
    auto size = QLabel::minimumSizeHint();
    return { size.width() + extentedWidth(), size.height() };
}

/*!
    \brief      初始化.
*/
void QCtmEditLabel::init()
{
    setAttribute(Qt::WA_Hover);
    QHBoxLayout* layout = new QHBoxLayout(this);
    m_impl->editButton  = new QToolButton(this);
    m_impl->editButton->setIcon(QIcon(":/QCustomUi/Resources/edit.png"));
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addStretch(1);
    layout->addWidget(m_impl->editButton);
    layout->setSpacing(EditButtonSpace);
    layout->addSpacing(EditButtonSize + EditButtonSpace);
    m_impl->editButton->setVisible(false);
    m_impl->editButton->setFixedSize(EditButtonSize, EditButtonSize);
    connect(m_impl->editButton, &QToolButton::clicked, this, &QCtmEditLabel::onEditButtonClicked);
}

/*!
    \brief      响应编辑按钮.
*/
void QCtmEditLabel::onEditButtonClicked()
{
    m_impl->editButton->hide();
    QWidget* editBase = new QWidget(this);
    editBase->setObjectName("QCtmEditLabel_editbase");
    editBase->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    editBase->setAttribute(Qt::WA_DeleteOnClose);
    editBase->setAttribute(Qt::WA_TranslucentBackground);
    editBase->setFixedSize(this->size());
    QHBoxLayout* layout = new QHBoxLayout(editBase);
    QLineEdit* edit     = new QLineEdit(editBase);
    edit->setFont(this->font());
    edit->setText(this->text());
    layout->addWidget(edit);
    auto applyBtn = new QToolButton(editBase);
    applyBtn->setObjectName("QCtmEditLabel_applyBtn");
    applyBtn->setFixedSize(EditButtonSize, EditButtonSize);
    applyBtn->setIcon(m_impl->finishButtonIcon);
    connect(applyBtn,
            &QToolButton::clicked,
            this,
            [=]()
            {
                this->setText(edit->text());
                editBase->close();
                emit editFinished();
            });
    layout->addWidget(applyBtn);
    auto cancelBtn = new QToolButton(editBase);
    cancelBtn->setObjectName("QCtmEditLabel_cancelBtn");
    cancelBtn->setFixedSize(EditButtonSize, EditButtonSize);
    cancelBtn->setIcon(m_impl->revertButtonIcon);
    connect(cancelBtn, &QToolButton::clicked, this, [=]() { editBase->close(); });
    layout->addWidget(cancelBtn);
    layout->setSpacing(EditButtonSpace - 2);
    layout->setContentsMargins(0, 0, 0, 0);
    editBase->move(this->parentWidget() ? this->parentWidget()->mapToGlobal(this->pos()) : this->pos());
    editBase->show();
}
