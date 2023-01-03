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

#include <QDialog>
#include <QMovie>

class QCUSTOMUI_EXPORT QCtmLoadingDialog : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool cancelEnable READ cancelEnable WRITE setCancelEnable)
public:
    enum class Result
    {
        Cancel,
        Done
    };
    QCtmLoadingDialog(QWidget* parent);
    ~QCtmLoadingDialog();

    void setCancelEnable(bool flag);
    bool cancelEnable() const;
    void setMovie(QMovie* movie);
    QMovie* movie() const;
public slots:
    void done();
    void cancel();
    Result exec();

protected:
    void showEvent(QShowEvent*) override;
    void closeEvent(QCloseEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event);
    void keyPressEvent(QKeyEvent* event) override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
