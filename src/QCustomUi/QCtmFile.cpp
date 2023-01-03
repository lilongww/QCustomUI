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
#include "QCtmFile.h"

/*!
    \class      QCtmFile
    \brief      文件操作类，提供文件读写的便捷方法.
    \inherits   QFile
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmFile.h
*/

/*!
    \brief      加载文件 \a file, 可设置文件打开模式 \a mode, 如果读取文件失败返回 std::nullopt，读取成功返回文件内容.
    \sa         save
*/
std::optional<QByteArray> QCtmFile::load(const QString& file, QIODevice::OpenMode mode)
{
    if (!mode.testFlag(QIODevice::ReadOnly))
        return std::nullopt;
    QFile f(file);
    if (!f.open(mode))
        return std::nullopt;
    return f.readAll();
}

/*!
    \brief      保存内容 \a content 到文件 \a file 中，可设置文件打开模式 \a mode, 如果保存成功则返回true，否则返回false.
    \sa         load
*/
bool QCtmFile::save(const QString& file, const QByteArray& content, QIODevice::OpenMode mode /*= QIODevice::WriteOnly*/)
{
    if (!mode.testFlag(QIODevice::WriteOnly))
        return false;
    QFile f(file);
    if (!f.open(mode))
        return false;
    f.write(content);
    return true;
}
