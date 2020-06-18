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

#include "QCtmMessageBox.h"
#include "QCtmTitleBar.h"
#include "Private/QCtmStyle_p.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>

struct QCtmMessageBox::Impl
{
    QLabel* message{ nullptr };
    QLabel* icon{ nullptr };
    QDialogButtonBox* buttonBox{ nullptr };
    QAbstractButton* clickedButton{ nullptr };
    QAbstractButton* defaultButton{ nullptr };
    QAbstractButton* escapeButton{ nullptr };
};

/*!
    \class      QCtmMessageBox
    \brief      QCtmMessageBox provide a custom looking message box, The usage is similar to \l QMessageBox.
    \inherits   QCtmDialog
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \sa         QMessageBox
*/

/*!
    \brief      Constructs a message box with the given \a parent.
*/
QCtmMessageBox::QCtmMessageBox(QWidget *parent)
    : QCtmDialog(parent)
    , m_impl(std::make_unique<Impl>())
{
    this->setStyle(new QCtmStyle(this->style()));
    init();
}

/*!
    \brief      Constructs a message box with the given \a icon, \a title, \a text, \a buttons, \a parent, \a f.
*/
QCtmMessageBox::QCtmMessageBox(Icon icon
    , const QString &title
    , const QString &text
    , StandardButtons buttons /*= NoButton */
    , QWidget *parent /*= Q_NULLPTR */
    , Qt::WindowFlags f /*= Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint*/)
    : QCtmMessageBox(parent)
{
    m_impl->buttonBox->setStandardButtons(QDialogButtonBox::StandardButtons((int)buttons));
    m_impl->message->setText(text);
    setWindowTitle(title);
    setWindowFlags(f | Qt::FramelessWindowHint | Qt::WindowCloseButtonHint);

    switch (icon)
    {
    case Information:
        m_impl->icon->setPixmap(this->style()->proxy()->standardPixmap(QStyle::SP_MessageBoxInformation));
        break;
    case Question:
        m_impl->icon->setPixmap(this->style()->proxy()->standardPixmap(QStyle::SP_MessageBoxQuestion));
        break;
    case Critical:
        m_impl->icon->setPixmap(this->style()->proxy()->standardPixmap(QStyle::SP_MessageBoxCritical));
        break;
    case Warning:
        m_impl->icon->setPixmap(this->style()->proxy()->standardPixmap(QStyle::SP_MessageBoxWarning));
        break;
    default:
        break;
    }
}

/*!
    \brief      Destorys the message box.
*/
QCtmMessageBox::~QCtmMessageBox()
{
}

/*!
    \brief      Sets the message box's default button to \a button.
    \sa         defaultButton
*/
void QCtmMessageBox::setDefaultButton(StandardButton button)
{
    auto btn = m_impl->buttonBox->button(QDialogButtonBox::StandardButton(button));
    if (btn)
    {
        btn->setFocus();
        m_impl->defaultButton = btn;
    }
}

/*!
    \brief      Returns the message box's default button.
    \sa         setDefaultButton
*/
QCtmMessageBox::StandardButton QCtmMessageBox::defaultButton()
{
    return QCtmMessageBox::StandardButton(m_impl->buttonBox->standardButton(m_impl->defaultButton));
}

/*!
    \brief      Returns the button that was clicked by the user, or nullptr if the user hit the Esc key and no escape button was set.
                If exec() hasn't been called yet, returns nullptr.
*/
QAbstractButton* QCtmMessageBox::clickedButton() const
{
    return m_impl->clickedButton;
}

/*!
    \brief      Returns the standard button enum value corresponding to the given \a button, or NoButton if the given button isn't a standard button.
*/
QCtmMessageBox::StandardButton QCtmMessageBox::standardButton(QAbstractButton* button) const
{
    if (!button)
    {
        detectEscapeButton();
        return QCtmMessageBox::StandardButton(m_impl->buttonBox->standardButton(m_impl->escapeButton));
    }
    return QCtmMessageBox::StandardButton(m_impl->buttonBox->standardButton(button));
}

/*!
    \brief      Adds the given \a button to the message box with the specified \a role.
    \sa         removeButton
*/
void QCtmMessageBox::addButton(QAbstractButton* button, ButtonRole role)
{
	if (!button)
		return;
	if (m_impl->buttonBox->buttons().contains(button))
		return;
	m_impl->buttonBox->addButton(button, (QDialogButtonBox::ButtonRole)role);
}

/*!
    \brief      Creates a button with the given \a text, adds it to the message box for the specified \a role, and returns it.
    \overload   addButton
    \sa         removeButton
*/
QPushButton* QCtmMessageBox::addButton(const QString& text, ButtonRole role)
{
	auto btn = new QPushButton(text, this);
	addButton(btn, role);
	return btn;
}

/*!
    \brief      Adds a standard button to the message box if it is valid to do so, and returns the push \a button.
    \overload   addButton
    \sa         removeButton
*/
QPushButton* QCtmMessageBox::addButton(StandardButton button)
{
	return m_impl->buttonBox->addButton((QDialogButtonBox::StandardButton)button);
}

/*!
    \brief      Removes button from the \a button box without deleting it.
    \sa         addButton
*/
void QCtmMessageBox::removeButton(QAbstractButton* button)
{
	if (!button)
		return;
	m_impl->buttonBox->removeButton(button);
}

/*!
    \brief      Constructs and show a message box by given \a parent, \a title, \a text, \a buttons, \a defaultButton.
    \sa         QMessageBox::critical
*/
QCtmMessageBox::StandardButton QCtmMessageBox::critical(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons /*= Ok */, StandardButton defaultButton /*= NoButton*/)
{
    QCtmMessageBox box(Icon::Critical, title, text, buttons, parent);
    box.setDefaultButton(defaultButton);
    box.exec();
    return box.standardButton(box.clickedButton());
}

/*!
    \brief      Constructs and show a message box by given \a parent, \a title, \a text, \a buttons, \a defaultButton.
    \sa         QMessageBox::information
*/
QCtmMessageBox::StandardButton QCtmMessageBox::information(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons /*= Ok */, StandardButton defaultButton /*= NoButton*/)
{
    QCtmMessageBox box(Icon::Information, title, text, buttons, parent);
    box.setDefaultButton(defaultButton);
    box.exec();
    return box.standardButton(box.clickedButton());
}

/*!
    \brief      Constructs and show a message box by given \a parent, \a title, \a text, \a buttons, \a defaultButton.
    \sa         QMessageBox::question
*/
QCtmMessageBox::StandardButton QCtmMessageBox::question(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons /*= StandardButtons(Yes | No) */, StandardButton defaultButton /*= NoButton*/)
{
    QCtmMessageBox box(Icon::Question, title, text, buttons, parent);
    box.setDefaultButton(defaultButton);
    box.exec();
    return box.standardButton(box.clickedButton());
}

/*!
    \brief      Constructs and show a message box by given \a parent, \a title, \a text, \a buttons, \a defaultButton.
    \sa         QMessageBox::warning
*/
QCtmMessageBox::StandardButton QCtmMessageBox::warning(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons /*= Ok */, StandardButton defaultButton /*= NoButton*/)
{
    QCtmMessageBox box(Icon::Warning, title, text, buttons, parent);
    box.setDefaultButton(defaultButton);
    box.exec();
    return box.standardButton(box.clickedButton());
}

/*!
    \reimp
*/
void QCtmMessageBox::showEvent(QShowEvent *event)
{
    updateSize();
    QCtmDialog::showEvent(event);
}

/*!
    \brief      Initialization
*/
void QCtmMessageBox::init()
{
    m_impl->message = new QLabel(this);
    m_impl->icon = new QLabel(this);
    m_impl->buttonBox = new QDialogButtonBox(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget());

    QHBoxLayout* msgLayout = new QHBoxLayout;
    msgLayout->addWidget(m_impl->icon, 0);
    msgLayout->addWidget(m_impl->message, 1);
    msgLayout->setMargin(0);
    msgLayout->setSpacing(20);
    layout->addLayout(msgLayout);
    layout->addWidget(m_impl->buttonBox);

    connect(m_impl->buttonBox, &QDialogButtonBox::clicked, this, [=](QAbstractButton* button) 
    {
        m_impl->clickedButton = button;
    });
    connect(m_impl->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_impl->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

/*!
    \brief      Calculate the minimum width of the message box.
    \sa         updateSize
*/
int QCtmMessageBox::layoutMinimumWidth()
{
    centralWidget()->layout()->activate();
    return centralWidget()->layout()->totalMinimumSize().width();
}

/*!
    \brief      Calculate the size of the message box.
    \sa         layoutMinimumWidth
*/
void QCtmMessageBox::updateSize()
{
    if (!isVisible())
        return;
    
    auto screen = qApp->screenAt(QCursor::pos());
    if(!screen)
        return;
    const auto& screenSize = screen->size();

    int hardLimit = qMin(screenSize.width() - 480, 1000); // can never get bigger than this
    // on small screens allows the messagebox be the same size as the screen
    if (screenSize.width() <= 1024)
        hardLimit = screenSize.width();
#ifdef Q_OS_MAC
    int softLimit = qMin(screenSize.width() / 2, 420);
#else
    // note: ideally on windows, hard and soft limits but it breaks compat
    int softLimit = qMin(screenSize.width() / 2, 500);
#endif

    m_impl->message->setWordWrap(false); // makes the label return min size
    int width = layoutMinimumWidth();

    if (width > softLimit) {
        m_impl->message->setWordWrap(true);
        width = qMax(softLimit, layoutMinimumWidth());
    }

    QFontMetrics fm(QApplication::font("QMdiSubWindowTitleBar"));
    int windowTitleWidth = qMin(fm.horizontalAdvance(windowTitle()) + 50, hardLimit);
    if (windowTitleWidth > width)
        width = windowTitleWidth;

    centralWidget()->layout()->activate();
    int height = (centralWidget()->layout()->hasHeightForWidth())
        ? centralWidget()->layout()->totalHeightForWidth(width)
        : centralWidget()->layout()->totalMinimumSize().height();

    const auto& margins = contentsMargins();
    setFixedSize(width + margins.left() + margins.right() + centralWidget()->layout()->margin()
        , height + this->titleBar()->height() + margins.bottom() + margins.top() + centralWidget()->layout()->margin());
}

/*!
    \brief      Calculate the escape button.
*/
void QCtmMessageBox::detectEscapeButton() const
{
    if (m_impl->escapeButton) { // escape button explicitly set
        return;
    }

    // Cancel button automatically becomes escape button
    m_impl->escapeButton = m_impl->buttonBox->button(QDialogButtonBox::Cancel);
    if (m_impl->escapeButton)
        return;

    // If there is only one button, make it the escape button
    const QList<QAbstractButton *> buttons = m_impl->buttonBox->buttons();
    if (buttons.count() == 1) {
        m_impl->escapeButton = buttons.first();
        return;
    }

    // if the message box has one RejectRole button, make it the escape button
    for (auto *button : buttons) {
        if (m_impl->buttonBox->buttonRole(button) == QDialogButtonBox::RejectRole) 
        {
            m_impl->escapeButton = button;
            if (m_impl->escapeButton)
                return;
        }
    }

    // if the message box has one NoRole button, make it the escape button
    for (auto *button : buttons) {
        if (m_impl->buttonBox->buttonRole(button) == QDialogButtonBox::NoRole)
        {
            m_impl->escapeButton = button;
            if (m_impl->escapeButton)
                return;
        }
    }
}
