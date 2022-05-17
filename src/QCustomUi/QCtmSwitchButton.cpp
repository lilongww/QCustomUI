#include "QCtmSwitchButton.h"

#include <QStyle>
#include <QStyleOption>
#include <QStylePainter>
#include <QVariantAnimation>

struct QCtmSwitchButton::Impl
{
    QBrush uncheckedBackground { 0xEDEDED };
    QColor uncheckedBackgroundBorder { 0xE0E0E0 };
    QBrush uncheckedHandle { 0xFFFFFF };
    QColor uncheckedHandleBorder { 0xE4E4E4 };
    QBrush checkedBackground { 0xFFFFFF };
    QColor checkedBackgroundBorder { Qt::transparent };
    QBrush checkedHandle { 0x42BF5F };
    QColor checkedHandleBorder { Qt::transparent };

    QVariantAnimation ani;

    QSize sizeHint;
};

/*!
    \class      QCtmSwitchButton
    \brief      状态切换按钮.
    \inherits   QAbstractButton
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmSwitchButton.h
*/

/*!
    \property   uncheckedBackground
    \brief      未选中状态的背景画刷.
*/

/*!
    \property   uncheckedBackgroundBorderColor
    \brief      未选中状态的背景边框颜色.
*/

/*!
    \property   uncheckedHandleBrush
    \brief      未选中状态的 handle 画刷.
*/

/*!
    \property   uncheckedHandleBorderColor
    \brief      未选中状态的 handle 边框颜色.
*/

/*!
    \property   checkedBackground
    \brief      选中状态的背景画刷.
*/

/*!
    \property   checkedBackgroundBorderColor
    \brief      选中状态的背景边框颜色.
*/

/*!
    \property   checkedHandleBrush
    \brief      选中状态的 handle 画刷.
*/

/*!
    \property   checkedHandleBorderColor
    \brief      选中状态的 handle 边框颜色.
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmSwitchButton::QCtmSwitchButton(QWidget* parent) : QAbstractButton(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->ani.setDuration(100);
    connect(&m_impl->ani, &QVariantAnimation::valueChanged, this, qOverload<void>(&QWidget::update));
    setCheckable(true);
    QVariant v(m_impl->checkedBackgroundBorder);
    auto vv = v.toString();
    connect(this,
            &QCtmSwitchButton::toggled,
            this,
            [=](bool checked)
            {
                m_impl->ani.setDirection(checked ? QAbstractAnimation::Direction::Forward : QAbstractAnimation::Direction::Backward);
                m_impl->ani.start();
            });
}

/*!
    \brief      析构函数.
*/
QCtmSwitchButton::~QCtmSwitchButton() {}

void QCtmSwitchButton::setUncheckedBackground(const QBrush& brush)
{
    m_impl->uncheckedBackground = brush;
    update();
}

const QBrush& QCtmSwitchButton::uncheckedBackground() const { return m_impl->uncheckedBackground; }

void QCtmSwitchButton::setUncheckedBackgroundBorderColor(const QColor& pen)
{
    m_impl->uncheckedBackgroundBorder = pen;
    update();
}

const QColor& QCtmSwitchButton::uncheckedBackgroundBorderColor() const { return m_impl->uncheckedBackgroundBorder; }

void QCtmSwitchButton::setUncheckedHandleBrush(const QBrush& brush)
{
    m_impl->uncheckedHandle = brush;
    update();
}

const QBrush& QCtmSwitchButton::uncheckedHandleBrush() const { return m_impl->uncheckedHandle; }

void QCtmSwitchButton::setUncheckedHandleBorderColor(const QColor& pen)
{
    m_impl->uncheckedHandleBorder = pen;
    update();
}

const QColor& QCtmSwitchButton::uncheckedHandleBorderColor() const { return m_impl->uncheckedHandleBorder; }

void QCtmSwitchButton::setCheckedBackground(const QBrush& brush)
{
    m_impl->checkedBackground = brush;
    update();
}

const QBrush& QCtmSwitchButton::checkedBackground() const { return m_impl->checkedBackground; }

void QCtmSwitchButton::setCheckedBackgroundBorderColor(const QColor& pen)
{
    m_impl->checkedBackgroundBorder = pen;
    update();
}

const QColor& QCtmSwitchButton::checkedBackgroundBorderColor() const { return m_impl->checkedBackgroundBorder; }

void QCtmSwitchButton::setCheckedHandleBrush(const QBrush& brush)
{
    m_impl->checkedHandle = brush;
    update();
}

const QBrush& QCtmSwitchButton::checkedHandleBrush() const { return m_impl->checkedHandle; }

void QCtmSwitchButton::setCheckedHandleBorderColor(const QColor& pen)
{
    m_impl->checkedHandleBorder = pen;
    update();
}

const QColor& QCtmSwitchButton::checkedHandleBorderColor() const { return m_impl->checkedHandleBorder; }

void QCtmSwitchButton::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    auto checked = this->isChecked();
    p.setPen(checked ? m_impl->checkedBackgroundBorder : m_impl->uncheckedBackgroundBorder);
    p.setBrush(checked ? m_impl->checkedBackground : m_impl->uncheckedBackground);
    p.drawRoundedRect(QRect(0, 0, width(), height()), height() / 2, height() / 2);
    p.setPen(checked ? m_impl->checkedHandleBorder : m_impl->uncheckedHandleBorder);
    p.setBrush(checked ? m_impl->checkedHandle : m_impl->uncheckedHandle);
    p.drawEllipse(m_impl->ani.currentValue().toRect());
}

void QCtmSwitchButton::resizeEvent(QResizeEvent* event)
{
    QAbstractButton::resizeEvent(event);
    m_impl->ani.setStartValue(QRect(1, 1, height() - 2, height() - 2));
    m_impl->ani.setEndValue(QRect(width() - height() + 1, 1, height() - 2, height() - 2));
}

QSize QCtmSwitchButton::minimumSizeHint() const
{
    if (!m_impl->sizeHint.isEmpty())
        return m_impl->sizeHint;
    QString s       = QStringLiteral("XXXX");
    QFontMetrics fm = fontMetrics();
    QSize sz        = fm.size(Qt::TextShowMnemonic, s);
    int w           = sz.width();
    int h           = sz.height();
    QStyleOptionButton opt;
    opt.initFrom(this);

    opt.rect.setSize(QSize(w, h)); // PM_MenuButtonIndicator depends on the height
    m_impl->sizeHint = style()->sizeFromContents(QStyle::CT_PushButton, &opt, QSize(w, h), this);
    return m_impl->sizeHint;
}

QSize QCtmSwitchButton::sizeHint() const { return minimumSizeHint(); }
