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

QCtmLoadingDialog::~QCtmLoadingDialog()
{
}

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
