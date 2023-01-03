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

#include "QCtmLoadingDialog.h"
#include "ui_QCtmLoadingDialog.h"

#include <QApplication>
#include <QDebug>
#include <QMovie>
#include <QResizeEvent>

struct QCtmLoadingDialog::Impl
{
    Ui::QCtmLoadingDialog* ui { new Ui::QCtmLoadingDialog };
    ~Impl() { delete ui; }
    QEventLoop loop;
    Result result { Result::Cancel };
    bool cancelEnable { false };
};

/*!
    \class      QCtmLoadingDialog
    \brief      等待加载窗口.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmLoadingDialog.h
*/

/*!
    \enum       QCtmLoadingDialog::Result
                loading窗口退出状态.
    \value      Cancel
                loading窗口以取消或中止状态退出.
    \value      Done
                loading窗口以完成状态退出.
*/

/*!
    \brief      构造函数，在 \a parent 的顶层窗口上覆盖加载窗口.
*/
QCtmLoadingDialog::QCtmLoadingDialog(QWidget* parent) : QWidget(parent->topLevelWidget()), m_impl(std::make_unique<Impl>())
{
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    m_impl->ui->setupUi(this);
    auto movie = new QMovie(this);
    movie->setFileName(":/QCustomUi/Resources/loading.gif");
    m_impl->ui->label->setMovie(movie);
    setVisible(false);
    parent->topLevelWidget()->installEventFilter(this);
}

/*!
    \brief      析构该窗口对象.
*/
QCtmLoadingDialog::~QCtmLoadingDialog() {}

/*!
    \brief      设置是否可以按ESC键取消 \a flag.
    \sa         cancelEnable
*/
void QCtmLoadingDialog::setCancelEnable(bool flag) { m_impl->cancelEnable = flag; }

/*!
    \brief      是否可以按ESC键取消显示loading窗口.
    \sa         setCancelEnable
*/
bool QCtmLoadingDialog::cancelEnable() const { return m_impl->cancelEnable; }

/*!
    \brief      设置动画 \a movie.
    \sa         movie
*/
void QCtmLoadingDialog::setMovie(QMovie* movie)
{
    if (auto old = m_impl->ui->label->movie(); old)
    {
        delete old;
    }
    m_impl->ui->label->setMovie(movie);
}

/*!
    \brief      返回动画指针.
    \sa         setMovie
*/
QMovie* QCtmLoadingDialog::movie() const { return m_impl->ui->label->movie(); }

/*!
    \brief      Loading 窗口加载完成时调用该函数，测试exec返回Done.
    \sa         cancel
*/
void QCtmLoadingDialog::done()
{
    QMetaObject::invokeMethod(this,
                              [=]
                              {
                                  m_impl->result = Result::Done;
                                  m_impl->loop.quit();
                              });
}

/*!
    \brief      Loading 窗口加载未完成时调用该函数，此时exec返回Cancel.
    \sa         done
*/
void QCtmLoadingDialog::cancel()
{
    QMetaObject::invokeMethod(this,
                              [=]
                              {
                                  m_impl->result = Result::Cancel;
                                  m_impl->loop.quit();
                              });
}

/*!
    \brief      阻塞并显示 loading 窗口.
    \note       不应使用show来显示loading.
*/
QCtmLoadingDialog::Result QCtmLoadingDialog::exec()
{
    m_impl->result = Result::Cancel;
    ensurePolished();
    setVisible(true);
    m_impl->loop.exec(QEventLoop::DialogExec);
    setVisible(false);
    m_impl->loop.processEvents();
    return m_impl->result;
}

/*!
    \reimp
*/
void QCtmLoadingDialog::showEvent(QShowEvent* e)
{
    auto tl = topLevelWidget();
    this->setGeometry(QRect(QPoint(0, 0), tl->size()));
    raise();
    setFocus();
    if (auto movie = m_impl->ui->label->movie(); movie)
    {
        movie->start();
    }
    QWidget::showEvent(e);
}

/*!
    \reimp
*/
void QCtmLoadingDialog::closeEvent(QCloseEvent* event)
{
    if (m_impl->loop.isRunning())
        m_impl->loop.quit();
    QWidget::closeEvent(event);
}

/*!
    \reimp
*/
void QCtmLoadingDialog::hideEvent(QHideEvent* event)
{
    if (m_impl->loop.isRunning())
        m_impl->loop.quit();
    QWidget::hideEvent(event);
}

/*!
    \reimp
*/
bool QCtmLoadingDialog::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == this->topLevelWidget())
    {
        if (event->type() == QEvent::Resize)
        {
            resize(this->topLevelWidget()->size());
        }
    }
    return false;
}

/*!
    \reimp
*/
void QCtmLoadingDialog::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape && m_impl->cancelEnable)
    {
        cancel();
    }
}
