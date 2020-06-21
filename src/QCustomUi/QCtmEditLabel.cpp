/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2020 LiLong                                              **
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
#include <QPainter>
#include <QToolButton>
#include <QHBoxLayout>
#include <QLineEdit>

constexpr int EditButtonSize = 16;
constexpr int EditButtonSpace = 4;
constexpr int extentedWidth() { return (EditButtonSpace + EditButtonSize) * 2; }

struct QCtmEditLabel::Impl
{
    bool readOnly{ false };
    QToolButton* editButton{ nullptr };
    QIcon finishButtonIcon{ ":/QCustomUi/Resources/ok.png" };
    QIcon revertButtonIcon{ ":/QCustomUi/Resources/cancel.png" };
};

/*!
    \class      QCtmEditLabel
    \brief      QCtmEditLabel is used for displaying text and edit the text.
    \inherits   QLabel
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \property   QCtmEditLabel::readOnly
    \brief      This property holds whether the line edit is read only.
                By default, this property is false.
*/

/*!
    \property   QCtmEditLabel::editButtonIcon
    \brief      This property holds edit button' icon.
*/

/*!
    \fn         void QCtmEditLabel::editFinished();
    \brief      Emit this signal when edit finished.
*/

/*!
    \brief      Constructs an empty label.
                The \a parent and widget flag \a f, arguments are passed to the QFrame constructor.
*/
QCtmEditLabel::QCtmEditLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent, f)
    , m_impl(std::make_unique<Impl>())
{
    init();
}

/*!
    \brief      Constructs a label that displays the text,\a text.
                The \a parent and widget flag \a f, arguments are passed to the QLabel constructor.
*/
QCtmEditLabel::QCtmEditLabel(const QString& text, QWidget* parent /*= nullptr*/, Qt::WindowFlags f)
    : QCtmEditLabel(parent, f)
{
    setText(text);
}

/*!
    \brief      Destroys the label.
*/
QCtmEditLabel::~QCtmEditLabel()
{

}

/*!
    \brief      Set the label is read only, \a flag.
    \sa         isReadOnly
*/
void QCtmEditLabel::setReadOnly(bool flag)
{
    m_impl->readOnly = flag;
}

/*!
    \brief      Returns whether the label is read-only
    \sa         setReadOnly
*/
bool QCtmEditLabel::isReadOnly() const
{
    return m_impl->readOnly;
}

/*!
    \brief      Set the edit button's \a icon
    \sa         editButtonIcon
*/
void QCtmEditLabel::setEditButtonIcon(const QIcon& icon)
{
    m_impl->editButton->setIcon(icon);
}

/*!
    \brief      Returns the edit button's icon
*/
QIcon QCtmEditLabel::editButtonIcon() const
{
    return m_impl->editButton->icon();
}

/*!
    \brief      Sets icon for the finish button.
    \sa         finishButtonIcon()
*/
void QCtmEditLabel::setFinishButtonIcon(const QIcon& icon)
{
    m_impl->finishButtonIcon = icon;
}

/*!
    \brief      Returns icon of finish button.
    \sa         setFinishButtonIcon
*/
QIcon QCtmEditLabel::finishButtonIcon() const
{
    return m_impl->finishButtonIcon;
}

/*!
    \brief      Sets icon for the revert button.
    \sa         revertButtonIcon()
*/
void QCtmEditLabel::setRevertButtonIcon(const QIcon& icon)
{
    m_impl->revertButtonIcon = icon;
}

/*!
    \brief      Returns icon of the revert button.
    \sa         setRevertButtonIcon
*/
QIcon QCtmEditLabel::revertButtonIcon() const
{
    return m_impl->revertButtonIcon;
}

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
    \brief      initialization
*/
void QCtmEditLabel::init()
{
    setAttribute(Qt::WA_Hover);
    QHBoxLayout* layout = new QHBoxLayout(this);
    m_impl->editButton = new QToolButton(this);
    m_impl->editButton->setIcon(QIcon(":/QCustomUi/Resources/edit.png"));
    layout->setMargin(0);
    layout->addStretch(1);
    layout->addWidget(m_impl->editButton);
    layout->setSpacing(EditButtonSpace);
    layout->addSpacing(EditButtonSize + EditButtonSpace);
    m_impl->editButton->setVisible(false);
    m_impl->editButton->setFixedSize(EditButtonSize, EditButtonSize);
    connect(m_impl->editButton, &QToolButton::clicked, this, &QCtmEditLabel::onEditButtonClicked);
}

/*!
    \brief      Edit button click operation.
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
    QLineEdit* edit = new QLineEdit(editBase);
    edit->setFont(this->font());
    edit->setText(this->text());
    layout->addWidget(edit);
    auto applayBtn = new QToolButton(editBase);
    applayBtn->setFixedSize(EditButtonSize, EditButtonSize);
    applayBtn->setIcon(m_impl->finishButtonIcon);
    connect(applayBtn, &QToolButton::clicked, this, [=]() {
        this->setText(edit->text());
        editBase->close();
        emit editFinished();
        });
    layout->addWidget(applayBtn);
    auto cancelBtn = new QToolButton(editBase);
    cancelBtn->setFixedSize(EditButtonSize, EditButtonSize);
    cancelBtn->setIcon(m_impl->revertButtonIcon);
    connect(cancelBtn, &QToolButton::clicked, this, [=]() {
        editBase->close();
        });
    layout->addWidget(cancelBtn);
    layout->setSpacing(EditButtonSpace - 2);
    layout->setMargin(0);
    editBase->move(this->parentWidget() ? this->parentWidget()->mapToGlobal(this->pos()) : this->pos());
    editBase->show();
}
