/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2020 LiLong                                              **
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

#include "QCtmLoadingDialog.h"
#include "ui_QCtmLoadingDialog.h"

#include <QApplication>
#include <QMovie>

struct QCtmLoadingDialog::Impl
{
    Ui::QCtmLoadingDialog* ui{ new Ui::QCtmLoadingDialog };
    QMovie* movie{ nullptr };
    ~Impl() { delete ui; }
};

/*!
    \class      QCtmLoadingDialog
    \brief      等待加载窗口.
    \inherits   QDialog
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmLoadingDialog.h
*/

/*!
    \brief      构造函数，在 \a parent 的顶层窗口上覆盖加载窗口.
*/
QCtmLoadingDialog::QCtmLoadingDialog(QWidget* parent)
    : QDialog(parent)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    m_impl->movie = new QMovie(this);
    m_impl->movie->setFileName(":/QCustomUi/Resources/loading.gif");
    m_impl->ui->label->setMovie(m_impl->movie);
}

/*!
    \brief      析构该窗口对象.
*/
QCtmLoadingDialog::~QCtmLoadingDialog()
{
}

/*!
    \reimp
*/
void QCtmLoadingDialog::showEvent(QShowEvent* e)
{
    auto pw = this->parentWidget();
    if (pw)
    {
        auto tl = pw->topLevelWidget();
        this->setGeometry(tl->geometry());
    }
    m_impl->movie->start();
    QDialog::showEvent(e);
}
