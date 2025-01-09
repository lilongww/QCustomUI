/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2025 LiLong                                              **
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
#include "QCtmSelectionButtonBox.h"
#include "Private/QCtmStyleOptionSelectionButtonBox.h"
#include "Private/QtVersionAdapter.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>

#include <algorithm>
#include <map>
#include <optional>

struct QCtmSelectionButtonBoxState
{
    bool checked { false };
    bool enabled { true };
    QString text;
};

struct QCtmSelectionButtonBox::Impl
{
    ExclusionPolicy policy { ExclusionPolicy::None };
    QVector<QCtmSelectionButtonBoxState> datas;
    bool uniformSize { true };
    Qt::Orientation orientation { Qt::Horizontal };
    std::optional<std::vector<QRect>> sizesCache;
    std::optional<QPoint> mousePos;
};

/*!
    \class      QCtmSelectionButtonBox
    \brief      选择按钮组合部件.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmSelectionButtonBox.h
*/

/*!
    \enum       QCtmSelectionButtonBox::ExclusionPolicy
                互斥策略.
    \value      None                不互斥.
    \value      Exclusive           互斥.
    \value      ExclusiveOptional   互斥，但可取消选中项.
*/

/*!
    \fn         void QCtmSelectionButtonBox::toggled(int index, bool checked)
    \brief      选项状态切换时发送该信号 \a index \a checked.
    \sa         setChecked, isChecked
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmSelectionButtonBox::QCtmSelectionButtonBox(QWidget* parent /*= nullptr*/) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed, QSizePolicy::PushButton));
    setMouseTracking(true);
}

/*!
    \brief      构造函数重载 \a orientation, \a parent.
*/
QCtmSelectionButtonBox::QCtmSelectionButtonBox(Qt::Orientation orientation, QWidget* parent) : QCtmSelectionButtonBox(parent)
{
    m_impl->orientation = orientation;
}

/*!
    \brief      构造函数重载 \a texts, \a orientation, \a parent.
*/
QCtmSelectionButtonBox::QCtmSelectionButtonBox(const QStringList& texts, Qt::Orientation orientation, QWidget* parent /*= nullptr*/)
    : QCtmSelectionButtonBox(orientation, parent)
{
    setTexts(texts);
}

/*!
    \brief      析构函数.
*/
QCtmSelectionButtonBox::~QCtmSelectionButtonBox() {}

/*!
    \brief      设置互斥策略 \a policy.
    \sa         exclusionPolicy
*/
void QCtmSelectionButtonBox::setExclusionPolicy(ExclusionPolicy policy) { m_impl->policy = policy; }

/*!
    \brief      返回互斥策略.
    \sa         setExclusionPolicy
*/
QCtmSelectionButtonBox::ExclusionPolicy QCtmSelectionButtonBox::exclusionPolicy() const { return m_impl->policy; }

/*!
    \brief      设置 \a index 选项的文本 \a text.
    \sa         text
*/
void QCtmSelectionButtonBox::setText(int index, const QString& text)
{
    if (index < 0)
        return;
    if (m_impl->datas.size() <= index)
        m_impl->datas.resize(index);
    m_impl->datas[index].text = text;
    updateGeometry();
}

/*!
    \brief      移除 \a index 选项.
*/
void QCtmSelectionButtonBox::remove(int index)
{
    m_impl->datas.removeAt(index);
    updateGeometry();
}

/*!
    \brief      设置所有选项文本 \a texts.
    \sa         texts
*/
void QCtmSelectionButtonBox::setTexts(const QStringList& texts)
{
    m_impl->datas.resize(texts.size());
    for (int i = 0; i < texts.size(); ++i)
    {
        m_impl->datas[i].text = texts[i];
    }
    updateGeometry();
}

/*!
    \brief      设置 \a index 选项选中状态 \a checked.
    \sa         isChecked
*/
void QCtmSelectionButtonBox::setChecked(int index, bool checked /*= true*/)
{
    auto& data = m_impl->datas[index];
    if (data.checked == checked)
        return;
    data.checked = checked;
    emit toggled(index, checked);
    if (checked)
    {
        switch (m_impl->policy)
        {
        case ExclusionPolicy::Exclusive:
        case ExclusionPolicy::ExclusiveOptional:
            for (int i = 0; i < m_impl->datas.size(); ++i)
            {
                if (i == index)
                    continue;
                setChecked(i, false);
            }
            break;
        }
    }
    update();
}

/*!
    \brief      返回 \a index 选项的文本.
    \sa         setText
*/
QString QCtmSelectionButtonBox::text(int index) const
{
    if (m_impl->datas.size() <= index || index < 0)
        return {};
    return m_impl->datas[index].text;
}

/*!
    \brief      返回所有选项的文本.
    \sa         setTexts
*/
QStringList QCtmSelectionButtonBox::texts() const
{
    QStringList ret;
    std::transform(m_impl->datas.begin(), m_impl->datas.end(), std::back_inserter(ret), [](const auto& data) { return data.text; });
    return ret;
}

/*!
    \brief      设置所有选项的大小是否一致 \a u.
    \sa         uniformSize
*/
void QCtmSelectionButtonBox::setUniformSize(bool u)
{
    if (m_impl->uniformSize == u)
        return;
    m_impl->uniformSize = u;
    updateGeometry();
}

/*!
    \brief      返回所有选项的文本是否一致.
    \sa         setUniformSize
*/
bool QCtmSelectionButtonBox::uniformSize() const { return m_impl->uniformSize; }

/*!
    \brief      返回 \a index 选项是否选中.
    \sa         setChecked
*/
bool QCtmSelectionButtonBox::isChecked(int index) const { return m_impl->datas[index].checked; }

/*!
    \brief      返回首个选中项，如果没有选中项则返回-1.
*/
int QCtmSelectionButtonBox::firstChecked() const
{
    for (int i = 0; i < m_impl->datas.size(); ++i)
    {
        if (m_impl->datas.at(i).checked)
            return i;
    }
    return -1;
}

/*!
    \brief      设置排布方向 \a orientation.
    \sa         orientation
*/
void QCtmSelectionButtonBox::setOrientation(Qt::Orientation orientation)
{
    if (m_impl->orientation == orientation)
        return;
    m_impl->orientation = orientation;
    if (m_impl->orientation == Qt::Horizontal)
        setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed, QSizePolicy::PushButton));
    else
        setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum, QSizePolicy::PushButton));
    updateGeometry();
}

/*!
    \brief      返回排布方向.
    \sa         setOrientation
*/
Qt::Orientation QCtmSelectionButtonBox::orientation() const { return m_impl->orientation; }

/*!
    \brief      返回选项数量.
*/
int QCtmSelectionButtonBox::count() const { return m_impl->datas.size(); }

/*!
    \reimp
*/
void QCtmSelectionButtonBox::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    QStyleOption opt;
    opt.initFrom(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    for (int i = 0; i < m_impl->datas.size(); ++i)
    {
        QCtmStyleOptionSelectionButtonBox opt;
        initStyleOption(i, opt);
        this->style()->drawControl(QStyle::CE_Header, &opt, &p, this);
    }
}

/*!
    \reimp
*/
void QCtmSelectionButtonBox::mouseReleaseEvent(QMouseEvent* event)
{
    switch (m_impl->policy)
    {
    case ExclusionPolicy::Exclusive:
        for (int i = 0; i < m_impl->datas.size(); ++i)
        {
            if (calcSizes()[i].contains(QtVersionAdapter::eventPos(event)))
            {
                setChecked(i, true);
                break;
            }
        }
        break;
    case ExclusionPolicy::None:
        for (int i = 0; i < m_impl->datas.size(); ++i)
        {
            if (calcSizes()[i].contains(QtVersionAdapter::eventPos(event)))
            {
                setChecked(i, !m_impl->datas[i].checked);
                break;
            }
        }
        break;
    case ExclusionPolicy::ExclusiveOptional:
        for (int i = 0; i < m_impl->datas.size(); ++i)
        {
            if (calcSizes()[i].contains(QtVersionAdapter::eventPos(event)))
            {
                setChecked(i, !m_impl->datas[i].checked);
                break;
            }
        }
        break;
    }
    update();
}

/*!
    \reimp
*/
void QCtmSelectionButtonBox::mouseMoveEvent(QMouseEvent* event)
{
    m_impl->mousePos = QtVersionAdapter::eventPos(event);
    update();
    QWidget::mouseMoveEvent(event);
}

/*!
    \reimp
*/
void QCtmSelectionButtonBox::leaveEvent(QEvent* event)
{
    m_impl->mousePos = std::nullopt;
    QWidget::leaveEvent(event);
}

/*!
    \reimp
*/
void QCtmSelectionButtonBox::resizeEvent(QResizeEvent* event)
{
    m_impl->sizesCache = std::nullopt;
    QWidget::resizeEvent(event);
}

/*!
    \reimp
*/
QSize QCtmSelectionButtonBox::minimumSizeHint() const
{
    if (m_impl->datas.isEmpty())
        return {};

    QCtmStyleOptionSelectionButtonBox opt;
    opt.text        = "XXXX";
    opt.orientation = Qt::Horizontal;
    auto height     = style()->sizeFromContents(QStyle::CT_HeaderSection, &opt, QSize(), this).height();

    if (m_impl->orientation == Qt::Horizontal)
    {
        if (m_impl->uniformSize)
        {
            int width = 0;
            for (int i = 0; i < m_impl->datas.size(); ++i)
            {
                const auto& data = m_impl->datas[i];
                width            = std::max(width, calcWidth(data.text));
            }
            return QSize(width * m_impl->datas.size(), height);
        }
        else
        {
            int width = std::accumulate(
                m_impl->datas.begin(), m_impl->datas.end(), 0, [&](int w, const auto& data) { return w + calcWidth(data.text); });
            return QSize(width, height);
        }
    }
    else
    {
        int width = 0;
        for (int i = 0; i < m_impl->datas.size(); ++i)
        {
            const auto& data = m_impl->datas[i];
            width            = std::max(width, calcWidth(data.text));
        }
        return QSize(width, height * m_impl->datas.size());
    }
}

/*!
    \reimp
*/
QSize QCtmSelectionButtonBox::sizeHint() const { return minimumSizeHint(); }

std::vector<QRect> QCtmSelectionButtonBox::calcSizes() const
{
    if (m_impl->sizesCache)
        return *m_impl->sizesCache;
    std::vector<QRect> ret(m_impl->datas.size());
    int offset = 0;
    if (m_impl->orientation == Qt::Horizontal)
    {
        auto minWidth = minimumSizeHint().width();
        if (!m_impl->uniformSize)
        {
            int step = static_cast<int>((width() - minWidth) / ret.size());
            for (int i = 0; i < ret.size(); ++i)
            {
                const auto& data = m_impl->datas[i];
                ret[i]           = QRect(offset, 0, calcWidth(data.text) + step, height());
                offset += ret[i].width();
            }
        }
        else // 统一宽度
        {
            for (int i = 0; i < ret.size(); ++i)
            {
                const auto& data = m_impl->datas[i];
                ret[i]           = QRect(offset, 0, static_cast<int>(width() / ret.size()), height());
                offset += ret[i].width();
            }
        }
        if (ret.size() > 1)
            ret.back().setWidth(
                this->width() -
                std::accumulate(ret.begin(), (ret.rbegin() + 1).base(), 0, [](int w, const auto& rect) { return w + rect.width(); }));
    }
    else
    {
        for (auto& rect : ret)
        {
            rect = QRect(0, offset, width(), this->height() / m_impl->datas.size());
            offset += rect.height();
        }
        if (ret.size() > 1)
            ret.back().setHeight(
                this->height() -
                std::accumulate(ret.begin(), (ret.rbegin() + 1).base(), 0, [](int w, const auto& rect) { return w + rect.height(); }));
    }
    m_impl->sizesCache = ret;
    return ret;
}

void QCtmSelectionButtonBox::initStyleOption(int index, QCtmStyleOptionSelectionButtonBox& opt) const
{
    const auto& state = m_impl->datas[index];
    opt.initFrom(this);
    opt.rect = calcSizes()[index];
    opt.state.setFlag(QStyle::State_Enabled, state.enabled);
    opt.state.setFlag(QStyle::State_Selected, state.checked);
    opt.state.setFlag(QStyle::State_MouseOver, m_impl->mousePos && opt.rect.contains(*m_impl->mousePos));
    opt.text = m_impl->datas[index].text;
    opt.state.setFlag(QStyle::State_HasFocus, false);
    opt.iconAlignment = Qt::AlignVCenter;
    bool first        = index == 0;
    bool last         = index == m_impl->datas.size() - 1;
    if (first && last)
        opt.position = QStyleOptionHeader::OnlyOneSection;
    else if (first)
        opt.position = QStyleOptionHeader::Beginning;
    else if (last)
        opt.position = QStyleOptionHeader::End;
    else
        opt.position = QStyleOptionHeader::Middle;
    opt.textAlignment = Qt::AlignCenter;
    opt.orientation   = m_impl->orientation;
}

int QCtmSelectionButtonBox::calcWidth(const QString& text) const
{
    QCtmStyleOptionSelectionButtonBox opt;
    opt.initFrom(this);
    opt.iconAlignment = Qt::AlignVCenter;
    opt.text          = text;
    opt.orientation   = Qt::Horizontal;
    opt.textAlignment = Qt::AlignCenter;
    auto w            = opt.fontMetrics.horizontalAdvance(text);
    return style()->sizeFromContents(QStyle::CT_HeaderSection, &opt, QSize(), this).width();
}
