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

#pragma once

#include <QSignalBlocker>

#include <vector>

class QCtmSignalBlocker
{
public:
    template<typename... Args>
    inline QCtmSignalBlocker(Args&&... objs);
    inline ~QCtmSignalBlocker() {}
    inline QCtmSignalBlocker(QCtmSignalBlocker&& other) noexcept { std::swap(other.m_blockers, m_blockers); }
    inline QCtmSignalBlocker& operator=(QCtmSignalBlocker&& other) noexcept { std::swap(other.m_blockers, m_blockers); }
    inline void reblock() noexcept;
    inline void unblock() noexcept;

private:
    Q_DISABLE_COPY(QCtmSignalBlocker)
    template<typename T>
    inline void emplaceBack(const T& obj);

private:
    std::vector<QSignalBlocker> m_blockers;
};

template<typename... Args>
QCtmSignalBlocker::QCtmSignalBlocker(Args&&... objs)
{
    (..., emplaceBack(objs));
}

void QCtmSignalBlocker::reblock() noexcept
{
    for (auto& blocker : m_blockers)
        blocker.reblock();
}

void QCtmSignalBlocker::unblock() noexcept
{
    for (auto& blocker : m_blockers)
        blocker.unblock();
}

template<typename T>
void QCtmSignalBlocker::emplaceBack(const T& obj)
{
    static_assert(std::is_base_of_v<QObject, std::remove_pointer_t<std::decay_t<T>>>);
    m_blockers.emplace_back(QSignalBlocker(obj));
}
