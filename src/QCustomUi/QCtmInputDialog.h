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

#include "qcustomui_global.h"

#include <QInputDialog>

class QCtmTitleBar;

class QCUSTOMUI_EXPORT QCtmInputDialog : public QInputDialog
{
public:
    QCtmInputDialog(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~QCtmInputDialog();

    void setTitleBar(QCtmTitleBar* titleBar);
    QCtmTitleBar* titleBar() const;
    void removeTitleBar();

    static QString getText(QWidget* parent,
                           const QString& title,
                           const QString& label,
                           QLineEdit::EchoMode echo              = QLineEdit::Normal,
                           const QString& text                   = QString(),
                           bool* ok                              = nullptr,
                           Qt::WindowFlags flags                 = Qt::WindowFlags(),
                           Qt::InputMethodHints inputMethodHints = Qt::ImhNone);
    static QString getMultiLineText(QWidget* parent,
                                    const QString& title,
                                    const QString& label,
                                    const QString& text                   = QString(),
                                    bool* ok                              = nullptr,
                                    Qt::WindowFlags flags                 = Qt::WindowFlags(),
                                    Qt::InputMethodHints inputMethodHints = Qt::ImhNone);
    static QString getItem(QWidget* parent,
                           const QString& title,
                           const QString& label,
                           const QStringList& items,
                           int current                           = 0,
                           bool editable                         = true,
                           bool* ok                              = nullptr,
                           Qt::WindowFlags flags                 = Qt::WindowFlags(),
                           Qt::InputMethodHints inputMethodHints = Qt::ImhNone);
    static int getInt(QWidget* parent,
                      const QString& title,
                      const QString& label,
                      int value             = 0,
                      int minValue          = -2147483647,
                      int maxValue          = 2147483647,
                      int step              = 1,
                      bool* ok              = nullptr,
                      Qt::WindowFlags flags = Qt::WindowFlags());
    static double getDouble(QWidget* parent,
                            const QString& title,
                            const QString& label,
                            double value          = 0,
                            double minValue       = -2147483647,
                            double maxValue       = 2147483647,
                            int decimals          = 1,
                            bool* ok              = nullptr,
                            Qt::WindowFlags flags = Qt::WindowFlags(),
                            double step           = 1);

protected:
    void hideEvent(QHideEvent*) override;
    void showEvent(QShowEvent* e) override;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#else
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#endif
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
