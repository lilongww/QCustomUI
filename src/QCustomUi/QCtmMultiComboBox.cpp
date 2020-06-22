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

#include "QCtmMultiComboBox.h"

#include <QListView>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QFrame>
#include <QLineEdit>
#include <QElapsedTimer>
#include <QDebug>

class QCtmMultiComboBoxModel : public QStandardItemModel
{
public:
    using QStandardItemModel::QStandardItemModel;
    Qt::ItemFlags flags([[maybe_unused]] const QModelIndex& index) const override
    {
        return Qt::ItemIsEnabled;
    }
    QVariant data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const override
    {
        const auto& d = QStandardItemModel::data(index, role);
        if (role == Qt::CheckStateRole)
        {
            return d.isValid() ? d : Qt::Unchecked;
        }
        return d;
    }
};

struct QCtmMultiComboBox::Impl
{
    QListView* view;
    QCtmMultiComboBoxModel* model;
    QElapsedTimer timer;
    QWidget* contianer{ nullptr };
};

/*!
    \class      QCtmMultiComboBox
    \brief      QCtmMultiComboBox provide a combobox, that can select multi items.
    \inherits   QComboBox
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \brief      Constructs a combobox with \a parent.
*/
QCtmMultiComboBox::QCtmMultiComboBox(QWidget* parent)
    : QComboBox(parent)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->view = new QListView(this);
    this->setView(m_impl->view);
    m_impl->model = new QCtmMultiComboBoxModel(this);
    setModel(m_impl->model);
    const auto& cons = this->findChildren<QFrame*>();
    auto lineEdit = new QLineEdit(this);
    setLineEdit(lineEdit);
    lineEdit->setReadOnly(true);
    lineEdit->installEventFilter(this);
    for (const auto& con : cons)
    {
        if (con->metaObject()->className() == QStringLiteral("QComboBoxPrivateContainer"))
        {
            m_impl->contianer = con;
            m_impl->view->viewport()->removeEventFilter(con);
            m_impl->view->viewport()->installEventFilter(this);
            con->installEventFilter(this);
        }
    }
    m_impl->timer.start();
}

/*!
    \brief      Destroys the combobox.
*/
QCtmMultiComboBox::~QCtmMultiComboBox()
{
}

/*!
    \overload
    \brief      Sets the given \a model
*/
void QCtmMultiComboBox::setModel(QAbstractItemModel* model)
{
    QComboBox::setModel(model);
    connect(model, &QAbstractItemModel::dataChanged, this, [=]() {
        const auto& items = this->checkedItems();
        this->lineEdit()->setText(items.join(";"));
        });
}

/*!
    \overload
    \brief      Returns the model.
*/
QAbstractItemModel* QCtmMultiComboBox::model() const
{
    return QComboBox::model();
}

/*!
    \brief      Returns the checked items.
    \sa         checkedDatas(), setChecked
*/
QStringList QCtmMultiComboBox::checkedItems() const
{
    QStringList items;
    for (int row = 0; row < m_impl->model->rowCount(); row++)
    {
        auto index = m_impl->model->index(row, 0);
        if (index.data(Qt::CheckStateRole).toInt() == Qt::Checked)
            items.push_back(index.data(Qt::DisplayRole).toString());
    }
    return items;
}

/*!
    \brief      Returns the checked items datas.
    \sa         checkedItems(), setChecked
*/
QVariantList QCtmMultiComboBox::checkedDatas() const
{
    QVariantList datas;
    for (int row = 0; row < m_impl->model->rowCount(); row++)
    {
        auto index = m_impl->model->index(row, 0);
        if (index.data(Qt::CheckStateRole).toInt() == Qt::Checked)
            datas.push_back(index.data(Qt::UserRole));
    }
    return datas;
}

/*!
    \brief      Set check state of the \a index, \a checked.
    \sa         isChecked
*/
void QCtmMultiComboBox::setChecked(int index, bool checked)
{
    auto in = m_impl->model->index(index, 0);
    m_impl->model->setData(in, checked ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
}

/*!
    \brief      Returns check state of the \a index.
    \sa         setChecked
*/
bool QCtmMultiComboBox::isChecked(int index) const
{
    auto in = m_impl->model->index(index, 0);
    return in.data(Qt::CheckStateRole).toInt() == Qt::Checked;
}

/*!
    \reimp
*/
bool QCtmMultiComboBox::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_impl->view->viewport())
    {
        switch (event->type())
        {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseMove:
            return false;
        case QEvent::MouseButtonRelease:
        {
            QMouseEvent* evt = (QMouseEvent*)(event);
            auto index = this->view()->indexAt(evt->pos());
            if (index.isValid())
            {
                this->model()->setData(index
                    , index.data(Qt::CheckStateRole).toInt() == Qt::Checked
                    ? Qt::Unchecked : Qt::Checked
                    , Qt::CheckStateRole);
            }
            return false;
        }
        default:
            break;
        }
    }
    else if (watched == this->lineEdit())
    {
        switch (event->type())
        {
        case  QEvent::MouseButtonPress:
            if (m_impl->timer.elapsed() > 50)
            {
                showPopup();
            }
            break;
        default:
            break;
        }

    }
    else if (watched == m_impl->contianer)
    {
        if (event->type() == QEvent::Hide || event->type() == QEvent::Close)
        {
            m_impl->timer.restart();
        }
#ifdef Q_OS_LINUX
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            event->ignore();
            return true;
        }
#endif
    }
    return QComboBox::eventFilter(watched, event);
}

/*!
    \reimp
*/
void QCtmMultiComboBox::showEvent(QShowEvent* e)
{
    QComboBox::showEvent(e);
    if (this->lineEdit())
    {
        this->lineEdit()->setText(this->checkedItems().join(";"));
    }
}
