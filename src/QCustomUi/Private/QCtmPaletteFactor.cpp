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

#include "QCtmPaletteFactor.h"
#include "../QCtmNavigationBar.h"
#include "../QCtmTitleBar.h"
#include "QCtmMaximumSizeButton_p.h"

#include <functional>
#include <map>

struct QCtmPaletteFactor::Impl
{
    std::map<QString, std::function<void(QWidget*)>> funcs;
};

QCtmPaletteFactor::QCtmPaletteFactor() : m_impl(std::make_unique<Impl>())
{
    auto& funcs = m_impl->funcs;
    funcs.insert({ QCtmTitleBar::staticMetaObject.className(), std::bind_front(&QCtmPaletteFactor::initQCtmTitleBar, this) });
    funcs.insert({ QCtmNavigationBar::staticMetaObject.className(), std::bind_front(&QCtmPaletteFactor::initQCtmNavigationBar, this) });
}

void QCtmPaletteFactor::init(QWidget* widget)
{
    static QCtmPaletteFactor ins;
    if (auto it = ins.m_impl->funcs.find(widget->metaObject()->className()); it != ins.m_impl->funcs.end())
    {
        std::invoke(it->second, widget);
    }
}

void QCtmPaletteFactor::initQCtmTitleBar(QWidget* bar)
{
    auto palette = bar->palette();
    palette.setBrush(QPalette::Window, Qt::white);
    bar->setPalette(palette);
    if (auto minBtn = bar->findChild<QWidget*>("minimumSizeBtn"))
    {
        minBtn->setProperty("icon", QIcon(":/QCustomUi/Resources/minimize-light.png"));
        auto palette = minBtn->palette();
        palette.setBrush(QPalette::Button, Qt::NoBrush);

        minBtn->setPalette(palette);
    }
    if (auto maxBtn = bar->findChild<QCtmMaximumSizeButton*>("maximumSizeBtn"))
    {
        maxBtn->setMaximumSizedIcon(QIcon(":/QCustomUi/Resources/maximize-light.png"));
        maxBtn->setRevertIcon(QIcon(":/QCustomUi/Resources/maximize_max-light.png"));
    }
    if (auto minBtn = bar->findChild<QWidget*>("closeBtn"))
    {
        minBtn->setProperty("icon", QIcon(":/QCustomUi/Resources/close-light.png"));
    }
}

void QCtmPaletteFactor::initQCtmNavigationBar(QWidget* bar)
{
    auto palette = bar->palette();
    palette.setBrush(QPalette::WindowText, QColor(0x4F4F4F));
    palette.setBrush(QPalette::Window, QColor(0xD9D9D9));
    bar->setPalette(palette);
}
