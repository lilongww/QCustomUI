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
#include <QScreen>

struct QCtmMessageBox::Impl
{
    QLabel* label{ nullptr };
    QLabel* iconLabel{ nullptr };
    Icon icon{ Icon::NoIcon };
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

    \b          {The message box screenshot:}
    \image      QCtmMessageBoxDetail.png
*/

/*!
    \enum QCtmMessageBox::ButtonRole

    This enum describes the roles that can be used to describe buttons in
    the button box. Combinations of these roles are as flags used to
    describe different aspects of their behavior.

    \value InvalidRole The button is invalid.
    \value AcceptRole Clicking the button causes the dialog to be accepted
           (e.g. OK).
    \value RejectRole Clicking the button causes the dialog to be rejected
           (e.g. Cancel).
    \value DestructiveRole Clicking the button causes a destructive change
           (e.g. for Discarding Changes) and closes the dialog.
    \value ActionRole Clicking the button causes changes to the elements within
           the dialog.
    \value HelpRole The button can be clicked to request help.
    \value YesRole The button is a "Yes"-like button.
    \value NoRole The button is a "No"-like button.
    \value ApplyRole The button applies current changes.
    \value ResetRole The button resets the dialog's fields to default values.

    \omitvalue NRoles

    \sa StandardButton
*/

/*!
    \enum QCtmMessageBox::StandardButton

    These enums describe flags for standard buttons. Each button has a
    defined \l ButtonRole.

    \value Ok An "OK" button defined with the \l AcceptRole.
    \value Open An "Open" button defined with the \l AcceptRole.
    \value Save A "Save" button defined with the \l AcceptRole.
    \value Cancel A "Cancel" button defined with the \l RejectRole.
    \value Close A "Close" button defined with the \l RejectRole.
    \value Discard A "Discard" or "Don't Save" button, depending on the platform,
                    defined with the \l DestructiveRole.
    \value Apply An "Apply" button defined with the \l ApplyRole.
    \value Reset A "Reset" button defined with the \l ResetRole.
    \value RestoreDefaults A "Restore Defaults" button defined with the \l ResetRole.
    \value Help A "Help" button defined with the \l HelpRole.
    \value SaveAll A "Save All" button defined with the \l AcceptRole.
    \value Yes A "Yes" button defined with the \l YesRole.
    \value YesToAll A "Yes to All" button defined with the \l YesRole.
    \value No A "No" button defined with the \l NoRole.
    \value NoToAll A "No to All" button defined with the \l NoRole.
    \value Abort An "Abort" button defined with the \l RejectRole.
    \value Retry A "Retry" button defined with the \l AcceptRole.
    \value Ignore An "Ignore" button defined with the \l AcceptRole.

    \value NoButton An invalid button.

    \omitvalue FirstButton
    \omitvalue LastButton

    \sa ButtonRole, standardButtons
*/

/*!
    \enum QCtmMessageBox::Icon

    This enum has the following values:

    \value NoIcon the message box does not have any icon.

    \value Question an icon indicating that
    the message is asking a question.

    \value Information an icon indicating that
    the message is nothing out of the ordinary.

    \value Warning an icon indicating that the
    message is a warning, but can be dealt with.

    \value Critical an icon indicating that
    the message represents a critical problem.

*/

/*!
    \property   QCtmMessageBox::text
    \brief      This property holds the message box text to be displayed.
                The text will be interpreted either as a plain text or as rich text, depending on the text format setting (QCtmMessageBox::textFormat). The default setting is Qt::AutoText, i.e., the message box will try to auto-detect the format of the text.
                The default value of this property is an empty string.
*/

/*!
    \property QCtmMessageBox::icon
    \brief the message box's icon

    The icon of the message box can be specified with one of the
    values:

    \list
    \li QCtmMessageBox::NoIcon
    \li QCtmMessageBox::Question
    \li QCtmMessageBox::Information
    \li QCtmMessageBox::Warning
    \li QCtmMessageBox::Critical
    \endlist

    The default is QCtmMessageBox::NoIcon.

    The pixmap used to display the actual icon depends on the current
    \l{QWidget::style} {GUI style}. You can also set a custom pixmap
    for the icon by setting the \l{QCtmMessageBox::iconPixmap} {icon
    pixmap} property.

    \sa iconPixmap
*/

/*!
    \property QCtmMessageBox::iconPixmap
    \brief the current icon

    The icon currently used by the message box. Note that it's often
    hard to draw one pixmap that looks appropriate in all GUI styles;
    you may want to supply a different pixmap for each platform.

    By default, this property is undefined.

    \sa icon
*/

/*!
    \property QCtmMessageBox::textFormat
    \brief the format of the text displayed by the message box

    The current text format used by the message box. See the \l
    Qt::TextFormat enum for an explanation of the possible options.

    The default format is Qt::AutoText.

    \sa setText()
*/

/*!
    \property QCtmMessageBox::standardButtons
    \brief collection of standard buttons in the message box

    This property controls which standard buttons are used by the message box.

    By default, this property contains no standard buttons.

    \sa addButton
*/

/*!
    \property QMessageBox::textInteractionFlags

    Specifies how the label of the message box should interact with user
    input.

    The default value depends on the style.

    \sa QStyle::SH_MessageBox_TextInteractionFlags
*/

/*!
    \brief      Constructs a message box with the given \a parent.
*/
QCtmMessageBox::QCtmMessageBox(QWidget* parent)
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
    , const QString& title
    , const QString& text
    , StandardButtons buttons /*= NoButton */
    , QWidget* parent /*= Q_NULLPTR */
    , Qt::WindowFlags f /*= Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint*/)
    : QCtmMessageBox(parent)
{
    m_impl->buttonBox->setStandardButtons(QDialogButtonBox::StandardButtons((int)buttons));
    m_impl->label->setText(text);
    setWindowTitle(title);
    setWindowFlags(f | Qt::FramelessWindowHint | Qt::WindowCloseButtonHint);
    setIcon(icon);
}

/*!
    \brief      Destorys the message box.
*/
QCtmMessageBox::~QCtmMessageBox()
{
}

/*!
    \brief      Sets the given standard \a buttons.
    \sa         standardButtons()
*/
void QCtmMessageBox::setStandardButtons(StandardButtons buttons)
{
    m_impl->buttonBox->setStandardButtons(QDialogButtonBox::StandardButtons(int(buttons)));

    QList<QAbstractButton*> buttonList = m_impl->buttonBox->buttons();
    if (!buttonList.contains(m_impl->escapeButton))
        m_impl->escapeButton = nullptr;
    if (!buttonList.contains(m_impl->defaultButton))
        m_impl->defaultButton = nullptr;
    updateSize();
}

/*!
    \brief      Returns the standard buttons.
    \sa         setStandardButtons
*/
QCtmMessageBox::StandardButtons QCtmMessageBox::standardButtons() const
{
    return QCtmMessageBox::StandardButtons(int(m_impl->buttonBox->standardButtons()));
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
    \brief      Sets the given \a text.
    \sa         text()
*/
void QCtmMessageBox::setText(const QString& text)
{
    m_impl->label->setText(text);
}

/*!
    \brief      Returns the text.
    \sa         setText
*/
QString QCtmMessageBox::text() const
{
    return m_impl->label->text();
}

/*!
    \brief      Returns the icon.
    \sa         setIcon
*/
QCtmMessageBox::Icon QCtmMessageBox::icon() const
{
    return m_impl->icon;
}

/*!
    \brief      Sets the given \a icon.
    \sa         icon()
*/
void QCtmMessageBox::setIcon(Icon icon)
{
    m_impl->icon = icon;
    switch (icon)
    {
    case Information:
        m_impl->iconLabel->setPixmap(this->style()->proxy()->standardPixmap(QStyle::SP_MessageBoxInformation));
        break;
    case Question:
        m_impl->iconLabel->setPixmap(this->style()->proxy()->standardPixmap(QStyle::SP_MessageBoxQuestion));
        break;
    case Critical:
        m_impl->iconLabel->setPixmap(this->style()->proxy()->standardPixmap(QStyle::SP_MessageBoxCritical));
        break;
    case Warning:
        m_impl->iconLabel->setPixmap(this->style()->proxy()->standardPixmap(QStyle::SP_MessageBoxWarning));
        break;
    default:
        break;
    }
}

/*!
    \brief      Returns the icon pixmap.
    \sa         setIconPixmap
*/
QPixmap QCtmMessageBox::iconPixmap() const
{
    return m_impl->iconLabel->pixmap(Qt::ReturnByValue);
}

/*!
    \brief      Sets the given \a pixmap, the \l icon will be set to be Icon::NoIcon.
    \sa         iconPixmap
*/
void QCtmMessageBox::setIconPixmap(const QPixmap& pixmap)
{
    m_impl->icon = Icon::NoIcon;
    m_impl->label->setPixmap(pixmap);
}

/*!
    \brief      Returns the text format.
    \sa         setTextFormat
*/
Qt::TextFormat QCtmMessageBox::textFormat() const
{
    return m_impl->label->textFormat();
}

/*!
    \brief      Sets the give text \a format.
    \sa         textFormat()
*/
void QCtmMessageBox::setTextFormat(Qt::TextFormat format)
{
    m_impl->label->setTextFormat(format);
}

/*!
    \brief      Sets the given text interaction \a flags.
    \sa         textInteractionFlags()
*/
void QCtmMessageBox::setTextInteractionFlags(Qt::TextInteractionFlags flags)
{
    m_impl->label->setTextInteractionFlags(flags);
}

/*!
    \brief      Returns the given text interaction flags.
    \sa         setTextInteractionFlags
*/
Qt::TextInteractionFlags QCtmMessageBox::textInteractionFlags() const
{
    return m_impl->label->textInteractionFlags();
}

/*!
    \brief      Constructs and show a message box by given \a parent, \a title, \a text, \a buttons, \a defaultButton.
    \sa         QMessageBox::critical
*/
QCtmMessageBox::StandardButton QCtmMessageBox::critical(QWidget* parent, const QString& title, const QString& text, StandardButtons buttons /*= Ok */, StandardButton defaultButton /*= NoButton*/)
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
QCtmMessageBox::StandardButton QCtmMessageBox::information(QWidget* parent, const QString& title, const QString& text, StandardButtons buttons /*= Ok */, StandardButton defaultButton /*= NoButton*/)
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
QCtmMessageBox::StandardButton QCtmMessageBox::question(QWidget* parent, const QString& title, const QString& text, StandardButtons buttons /*= StandardButtons(Yes | No) */, StandardButton defaultButton /*= NoButton*/)
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
QCtmMessageBox::StandardButton QCtmMessageBox::warning(QWidget* parent, const QString& title, const QString& text, StandardButtons buttons /*= Ok */, StandardButton defaultButton /*= NoButton*/)
{
    QCtmMessageBox box(Icon::Warning, title, text, buttons, parent);
    box.setDefaultButton(defaultButton);
    box.exec();
    return box.standardButton(box.clickedButton());
}

/*!
    \reimp
*/
void QCtmMessageBox::showEvent(QShowEvent* event)
{
    updateSize();
    QCtmDialog::showEvent(event);
}

/*!
    \brief      Initialization
*/
void QCtmMessageBox::init()
{
    m_impl->label = new QLabel(this);
    m_impl->iconLabel = new QLabel(this);
    m_impl->buttonBox = new QDialogButtonBox(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget());

    QHBoxLayout* msgLayout = new QHBoxLayout;
    msgLayout->addWidget(m_impl->iconLabel, 0);
    msgLayout->addWidget(m_impl->label, 1);
    msgLayout->setContentsMargins(0,0,0,0);
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
    if (!screen)
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

    m_impl->label->setWordWrap(false); // makes the label return min size
    int width = layoutMinimumWidth();

    if (width > softLimit) {
        m_impl->label->setWordWrap(true);
        width = qMax(softLimit, layoutMinimumWidth());
    }

    QFontMetrics fm(QApplication::font("QMdiSubWindowTitleBar"));
    int windowTitleWidth = qMin(fm.horizontalAdvance(windowTitle()) + 50, hardLimit);
    if (windowTitleWidth > width)
        width = windowTitleWidth;

    auto layout = centralWidget()->layout();
    layout->activate();
    int height = (layout->hasHeightForWidth())
        ? layout->totalHeightForWidth(width)
        : layout->totalMinimumSize().height();

    const auto& margins = contentsMargins();
    const auto& layoutMargins = layout->contentsMargins();
    setFixedSize(width + margins.left() + margins.right() + layoutMargins.left() + layoutMargins.right()
        , height + this->titleBar()->height() + margins.bottom() + margins.top() + layoutMargins.top() + layoutMargins.bottom());
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
    const QList<QAbstractButton*> buttons = m_impl->buttonBox->buttons();
    if (buttons.count() == 1) {
        m_impl->escapeButton = buttons.first();
        return;
    }

    // if the message box has one RejectRole button, make it the escape button
    for (auto* button : buttons) {
        if (m_impl->buttonBox->buttonRole(button) == QDialogButtonBox::RejectRole)
        {
            m_impl->escapeButton = button;
            if (m_impl->escapeButton)
                return;
        }
    }

    // if the message box has one NoRole button, make it the escape button
    for (auto* button : buttons) {
        if (m_impl->buttonBox->buttonRole(button) == QDialogButtonBox::NoRole)
        {
            m_impl->escapeButton = button;
            if (m_impl->escapeButton)
                return;
        }
    }
}
