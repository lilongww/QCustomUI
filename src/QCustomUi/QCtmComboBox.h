#pragma once

#include "qcustomui_global.h"

#include <QComboBox>

class QCUSTOMUI_EXPORT QCtmComboBox : public QComboBox
{
    Q_OBJECT

public:
    QCtmComboBox(QWidget *parent);
    ~QCtmComboBox();
};
