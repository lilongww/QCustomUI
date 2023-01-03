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

#include "../QCtmMessageTipButton.h"
#include "QCtmMessageTipHelper_p.h"

#include <QEvent>
#include <QPainter>
#include <QStyleOption>

#include <assert.h>

struct QCtmMessageTipHelper::Impl
{
    QSize size { 14, 16 };
    int margin { 5 };
    int radius { 25 };

    QColor tipColor;
};

QCtmMessageTipHelper::QCtmMessageTipHelper(QCtmMessageTipButton* parent) : QObject(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->size = parent->fontMetrics().size(Qt::TextSingleLine, "99");
    parent->installEventFilter(this);
}

QCtmMessageTipHelper::~QCtmMessageTipHelper() {}

void QCtmMessageTipHelper::drawMessageTip(int msgCount, QPainter& p, QWidget* w)
{
    if (!msgCount)
        return;
    if (msgCount > 99)
        msgCount = 99;
    const auto& rect = tipsRect(QRect(QPoint(0, 0), w->size()));
    QStyleOption opt;
    opt.initFrom(w);
    p.save();
    p.setPen(Qt::NoPen);
    p.setBrush(m_impl->tipColor);
    p.drawRoundedRect(rect, m_impl->radius, m_impl->radius, Qt::RelativeSize);
    p.restore();

    QTextOption to;
    to.setAlignment(Qt::AlignCenter);
    p.save();
    p.setPen(opt.palette.windowText().color());
    p.drawText(rect, QString::number(msgCount), to);
    p.restore();
}

void QCtmMessageTipHelper::setTipColor(const QColor& color) { m_impl->tipColor = color; }

const QColor& QCtmMessageTipHelper::tipColor() const { return m_impl->tipColor; }

QRect QCtmMessageTipHelper::tipsRect(const QRect& rect)
{
    auto nrect = rect.normalized();
    return QRect(QPoint(nrect.right() - m_impl->size.width() - m_impl->margin, nrect.top() + m_impl->margin), m_impl->size);
}

bool QCtmMessageTipHelper::eventFilter(QObject* watched, QEvent* event)
{
    auto w = qobject_cast<QCtmMessageTipButton*>(watched);
    if (w && event->type() == QEvent::FontChange)
    {
        m_impl->size = w->fontMetrics().size(Qt::TextSingleLine, "99");
        ;
    }
    return false;
}
