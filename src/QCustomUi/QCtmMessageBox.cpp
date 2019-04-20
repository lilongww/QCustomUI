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

struct QCtmMessageBox::Impl
{
    QLabel* message{ nullptr };
    QLabel* icon{ nullptr };
    QDialogButtonBox* buttonBox{ nullptr };
    QAbstractButton* clickedButton{ nullptr };
    QAbstractButton* defaultButton{ nullptr };
    QAbstractButton* escapeButton{ nullptr };
};

QCtmMessageBox::QCtmMessageBox(QWidget *parent)
    : QCtmDialog(parent)
    , m_impl(std::make_shared<Impl>())
{
    this->setStyle(new QCtmStyle(this->style()));
    init();
}

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
    }
}

QCtmMessageBox::~QCtmMessageBox()
{
}

void QCtmMessageBox::setDefaultButton(StandardButton button)
{
    auto btn = m_impl->buttonBox->button(QDialogButtonBox::StandardButton(button));
    if (btn)
    {
        btn->setFocus();
        m_impl->defaultButton = btn;
    }
}

QCtmMessageBox::StandardButton QCtmMessageBox::defaultButton()
{
    return QCtmMessageBox::StandardButton(m_impl->buttonBox->standardButton(m_impl->defaultButton));
}

QAbstractButton* QCtmMessageBox::clickedButton() const
{
    return m_impl->clickedButton;
}

QCtmMessageBox::StandardButton QCtmMessageBox::standardButton(QAbstractButton* button) const
{
    if (!button)
    {
        detectEscapeButton();
        return QCtmMessageBox::StandardButton(m_impl->buttonBox->standardButton(m_impl->escapeButton));
    }
    return QCtmMessageBox::StandardButton(m_impl->buttonBox->standardButton(button));
}

QCtmMessageBox::StandardButton QCtmMessageBox::critical(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons /*= Ok */, StandardButton defaultButton /*= NoButton*/)
{
    QCtmMessageBox box(Icon::Critical, title, text, buttons, parent);
    box.setDefaultButton(defaultButton);
    box.exec();
    return box.standardButton(box.clickedButton());
}

QCtmMessageBox::StandardButton QCtmMessageBox::information(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons /*= Ok */, StandardButton defaultButton /*= NoButton*/)
{
    QCtmMessageBox box(Icon::Information, title, text, buttons, parent);
    box.setDefaultButton(defaultButton);
    box.exec();
    return box.standardButton(box.clickedButton());
}

QCtmMessageBox::StandardButton QCtmMessageBox::question(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons /*= StandardButtons(Yes | No) */, StandardButton defaultButton /*= NoButton*/)
{
    QCtmMessageBox box(Icon::Question, title, text, buttons, parent);
    box.setDefaultButton(defaultButton);
    box.exec();
    return box.standardButton(box.clickedButton());
}

QCtmMessageBox::StandardButton QCtmMessageBox::warning(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons /*= Ok */, StandardButton defaultButton /*= NoButton*/)
{
    QCtmMessageBox box(Icon::Warning, title, text, buttons, parent);
    box.setDefaultButton(defaultButton);
    box.exec();
    return box.standardButton(box.clickedButton());
}

void QCtmMessageBox::showEvent(QShowEvent *event)
{
    updateSize();
    QCtmDialog::showEvent(event);
}

void QCtmMessageBox::init()
{
    m_impl->message = new QLabel(this);
    m_impl->icon = new QLabel(this);
    m_impl->buttonBox = new QDialogButtonBox(this);
    QVBoxLayout* layout = new QVBoxLayout(content());

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

int QCtmMessageBox::layoutMinimumWidth()
{
    content()->layout()->activate();
    return content()->layout()->totalMinimumSize().width();
}

void QCtmMessageBox::updateSize()
{
    if (!isVisible())
        return;

    QSize screenSize = QApplication::desktop()->availableGeometry(QCursor::pos()).size();
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
    int windowTitleWidth = qMin(fm.width(windowTitle()) + 50, hardLimit);
    if (windowTitleWidth > width)
        width = windowTitleWidth;

    content()->layout()->activate();
    int height = (content()->layout()->hasHeightForWidth())
        ? content()->layout()->totalHeightForWidth(width)
        : content()->layout()->totalMinimumSize().height();

    int left, top, right, bottom;
    this->getContentsMargins(&left, &top, &right, &bottom);
    setFixedSize(width + left + right + content()->layout()->margin()
        , height + this->titleBar()->height() + bottom + top + content()->layout()->margin());
    //QCoreApplication::removePostedEvents(this, QEvent::LayoutRequest);
}

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