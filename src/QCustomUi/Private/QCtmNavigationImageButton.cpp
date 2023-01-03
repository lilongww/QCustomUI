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

#include "QCtmNavigationImageButton_p.h"

#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>

constexpr auto Padding = 10;
constexpr auto Margins = QMargins { Padding, Padding, Padding, Padding };

struct QCtmNavigationImageButton::Impl
{
    bool pressed = false;
    QPixmap cache;
    inline void generateCache(QCtmNavigationImageButton* q, const QSize& size)
    {
        auto st           = q->isChecked() ? QIcon::On : QIcon::Off;
        const auto& sizes = q->icon().availableSizes(QIcon::Mode::Normal, st);
        if ((cache.isNull() || cache.size().grownBy(Margins) != size) && !sizes.isEmpty())
        {
            cache = q->icon()
                        .pixmap(sizes.front(), QIcon::Normal, st)
                        .scaledToHeight(qMin(size.shrunkBy(Margins).height(), sizes.front().height()),
                                        Qt::TransformationMode::SmoothTransformation);
            q->updateGeometry();
        }
    }
};

QCtmNavigationImageButton::QCtmNavigationImageButton(QWidget* parent) : QCtmToolButton(parent), m_impl(std::make_unique<Impl>())
{
    this->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred));
    connect(this,
            &QCtmNavigationImageButton::pressed,
            this,
            [=]()
            {
                m_impl->pressed = true;
                update();
            });
    connect(this,
            &QCtmNavigationImageButton::released,
            this,
            [=]()
            {
                m_impl->pressed = false;
                update();
            });
}

QCtmNavigationImageButton::~QCtmNavigationImageButton() {}

void QCtmNavigationImageButton::paintEvent([[maybe_unused]] QPaintEvent* e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QStyleOptionButton opt;
    opt.initFrom(this);
    opt.features = QStyleOptionButton::DefaultButton | QStyleOptionButton::AutoDefaultButton;
    ;
    if (m_impl->pressed)
    {
        opt.state |= QStyle::State_Sunken;
        opt.state |= QStyle::State_Raised;
    }
    else if (this->isChecked())
    {
        opt.state |= QStyle::State_On;
    }
    opt.text = this->text();
    style()->drawControl(QStyle::CE_PushButton, &opt, &p, this);

    { // draw icon
        if (!m_impl->cache.isNull())
        {
            style()->drawItemPixmap(&p, opt.rect, Qt::AlignCenter, m_impl->cache);
        }
        else
        {
            m_impl->generateCache(this, this->size());
        }
    }
}

void QCtmNavigationImageButton::resizeEvent(QResizeEvent* e)
{
    m_impl->generateCache(this, e->size());
    QCtmToolButton::resizeEvent(e);
}

QSize QCtmNavigationImageButton::sizeHint() const
{
    if (!m_impl->cache.isNull())
    {
        return m_impl->cache.size().grownBy(Margins);
    }
    return QSize(5, 5);
}
