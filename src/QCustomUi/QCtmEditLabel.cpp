#include "QCtmEditLabel.h"

#include <QEvent>
#include <QPainter>
#include <QToolButton>
#include <QHBoxLayout>
#include <QLineEdit>

constexpr int EditButtonSize = 16;
constexpr int EditButtonSpace = 4;
constexpr int extentedWidth() { return (EditButtonSpace + EditButtonSize)*2; }

struct QCtmEditLabel::Impl
{
	bool editable{ true };
	QToolButton* editButton{ nullptr };
};

QCtmEditLabel::QCtmEditLabel(QWidget *parent, Qt::WindowFlags f)
	: QLabel(parent, f)
	, m_impl(std::make_unique<Impl>())
{
	init();
}

QCtmEditLabel::QCtmEditLabel(const QString& text, QWidget* parent /*= nullptr*/, Qt::WindowFlags f)
	: QCtmEditLabel(parent, f)
{
	setText(text);
}

QCtmEditLabel::~QCtmEditLabel()
{

}

void QCtmEditLabel::setEditable(bool flag)
{
	m_impl->editable = flag;
}

bool QCtmEditLabel::editable() const
{
	return m_impl->editable;
}

void QCtmEditLabel::setEditButtonIcon(const QIcon& icon)
{
	m_impl->editButton->setIcon(icon);
}

QIcon QCtmEditLabel::editButtonIcon() const
{
	return m_impl->editButton->icon();
}

bool QCtmEditLabel::event(QEvent* e)
{
	switch (e->type())
	{
	case QEvent::HoverEnter:
		if(m_impl->editable)
			m_impl->editButton->setVisible(true);
		break;
	case QEvent::HoverLeave:
		if(m_impl->editable || m_impl->editButton->isVisible())
			m_impl->editButton->setVisible(false);
		break;
	}
	return QLabel::event(e);
}

QSize QCtmEditLabel::minimumSizeHint() const
{
	auto size = QLabel::minimumSizeHint();
	return { size.width() + extentedWidth(), size.height() };
}

void QCtmEditLabel::init()
{
	setAttribute(Qt::WA_Hover);
	QHBoxLayout* layout = new QHBoxLayout(this);
	m_impl->editButton = new QToolButton(this);
	m_impl->editButton->setIcon(QIcon(":/QCustomUi/Resources/edit.png"));
	layout->setMargin(0);
	layout->addStretch(1);
	layout->addWidget(m_impl->editButton);
	layout->setSpacing(EditButtonSpace);
	layout->addSpacing(EditButtonSize + EditButtonSpace);
	m_impl->editButton->setVisible(false);
	m_impl->editButton->setFixedSize(EditButtonSize, EditButtonSize);
	connect(m_impl->editButton, &QToolButton::clicked, this, &QCtmEditLabel::onEditButtonClicked);
}

void QCtmEditLabel::onEditButtonClicked()
{
	m_impl->editButton->hide();
	QWidget *editBase = new QWidget(this);
	editBase->setObjectName("QCtmEditLabel_editbase");
	editBase->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	editBase->setAttribute(Qt::WA_DeleteOnClose);
	editBase->setAttribute(Qt::WA_TranslucentBackground);
	editBase->setFixedSize(this->size());
	QHBoxLayout* layout = new QHBoxLayout(editBase);
	QLineEdit* edit = new QLineEdit(editBase);
	edit->setFont(this->font());
	edit->setText(this->text());
	layout->addWidget(edit);
	auto applayBtn = new QToolButton(editBase);
	applayBtn->setFixedSize(EditButtonSize, EditButtonSize);
	applayBtn->setIcon(QIcon(":/QCustomUi/Resources/ok.png"));
	connect(applayBtn, &QToolButton::clicked, this, [=]() {
		this->setText(edit->text());
		editBase->close();
		emit editFinished();
		});
	layout->addWidget(applayBtn);
	auto cancelBtn = new QToolButton(editBase);
	cancelBtn->setFixedSize(EditButtonSize, EditButtonSize);
	cancelBtn->setIcon(QIcon(":/QCustomUi/Resources/cancel.png"));
	connect(cancelBtn, &QToolButton::clicked, this, [=]() {
		editBase->close();
		});
	layout->addWidget(cancelBtn);
	layout->setSpacing(EditButtonSpace-2);
	layout->setMargin(0);
	editBase->move(this->parentWidget() ? this->parentWidget()->mapToGlobal(this->pos()) : this->pos());
	editBase->show();
}
