#include "QCtmMultiComboBox.h"

#include <QListView>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QFrame>
#include <QLineEdit>
#include <QElapsedTimer>

class QCtmMultiComboBoxModel : public QStandardItemModel
{
public:
	using QStandardItemModel::QStandardItemModel;
	Qt::ItemFlags flags(const QModelIndex &index) const override
	{
		return Qt::ItemIsEnabled;
	}
	QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const override
	{
		auto d = QStandardItemModel::data(index, role);
		if (role == Qt::CheckStateRole)
		{
			return d.isValid() ? d: Qt::Unchecked;
		}
		return d;
	}
};

struct QCtmMultiComboBox::Impl
{
	QListView* view;
	QCtmMultiComboBoxModel *model;
	QElapsedTimer timer;
	QWidget* contianer{ nullptr };
};

QCtmMultiComboBox::QCtmMultiComboBox(QWidget *parent)
	: QComboBox(parent)
	, m_impl(std::make_unique<Impl>())
{
	m_impl->view = new QListView(this);
	this->setView(m_impl->view);
	m_impl->model = new QCtmMultiComboBoxModel(this);
	setModel(m_impl->model);
	auto cons = this->findChildren<QFrame*>();
	auto lineEdit = new QLineEdit(this);
	setLineEdit(lineEdit);
	lineEdit->setReadOnly(true);
	lineEdit->installEventFilter(this);
	for (auto con : cons)
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

QCtmMultiComboBox::~QCtmMultiComboBox()
{
}

void QCtmMultiComboBox::setModel(QAbstractItemModel* model)
{
	QComboBox::setModel(model);
	connect(model, &QAbstractItemModel::dataChanged, this, [=]() {
		auto &&items = this->checkedItems();
		this->lineEdit()->setText(items.join(";"));
	});
}

QAbstractItemModel* QCtmMultiComboBox::model() const
{
	return QComboBox::model();
}

QStringList QCtmMultiComboBox::checkedItems() const
{
	QStringList items;
	for (int row = 0; row < m_impl->model->rowCount(); row++)
	{
		auto index = m_impl->model->index(row, 0);
		if(index.data(Qt::CheckStateRole).toInt() == Qt::Checked)
			items.push_back(index.data(Qt::DisplayRole).toString());
	}
	return std::move(items);
}

QVariantList QCtmMultiComboBox::checkedDatas() const
{
	QVariantList datas;
	for (int row = 0; row < m_impl->model->rowCount(); row++)
	{
		auto index = m_impl->model->index(row, 0);
		if (index.data(Qt::CheckStateRole).toInt() == Qt::Checked)
			datas.push_back(index.data(Qt::UserRole));
	}
	return std::move(datas);
}

void QCtmMultiComboBox::setChecked(int index, bool checked)
{
	auto in = m_impl->model->index(index, 0);
	m_impl->model->setData(in, checked ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
}

bool QCtmMultiComboBox::isChecked(int index) const
{
	auto in = m_impl->model->index(index, 0);
	return in.data(Qt::CheckStateRole).toInt() == Qt::Checked;
}

bool QCtmMultiComboBox::eventFilter(QObject *watched, QEvent *event)
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
		}
	}
	else if (watched == this->lineEdit())
	{
		switch (event->type())
		{
		case  QEvent::MouseButtonPress:
			if (m_impl->timer.elapsed() > 50)
			{
				this->showPopup();
			}
		}

	}
	else if (watched == m_impl->contianer 
		&& event->type() == QEvent::Hide 
		|| event->type() == QEvent::Close)
	{
		m_impl->timer.restart();
	}
	return QComboBox::eventFilter(watched, event);
}

void QCtmMultiComboBox::showEvent(QShowEvent *e)
{
	QComboBox::showEvent(e);
	if (this->lineEdit())
	{
		this->lineEdit()->setText(this->checkedItems().join(";"));
	}
}
