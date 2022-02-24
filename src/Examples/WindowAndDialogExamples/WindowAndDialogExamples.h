#pragma once

#include "ui_WindowAndDialogExamples.h"

#include <QCustomUi/QCtmWindow.h>

class WindowAndDialogExamples : public QCtmWindow
{
    Q_OBJECT

public:
    WindowAndDialogExamples(QWidget* parent = Q_NULLPTR);

private:
    Ui::WindowAndDialogExamplesClass ui;
};
