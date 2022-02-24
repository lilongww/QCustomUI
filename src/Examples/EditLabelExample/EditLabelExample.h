#pragma once

#include <QCustomUi/QCtmWindow.h>

class EditLabelExample : public QCtmWindow
{
    Q_OBJECT

public:
    EditLabelExample(QWidget* parent = Q_NULLPTR);
    ~EditLabelExample();

private:
    void init();
};
