#include "QCtmComboBox.h"

#include <QListView>

QCtmComboBox::QCtmComboBox(QWidget *parent)
    : QComboBox(parent)
{
    setView(new QListView(this));
}

QCtmComboBox::~QCtmComboBox()
{
}
