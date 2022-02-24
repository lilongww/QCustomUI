#include "InputDialogExample.h"

#include <QCustomUi/QCtmInputDialog.h>

InputDialogExample::InputDialogExample(QWidget* parent) : QCtmWindow(parent)
{
    ui.setupUi(this->centralWidget());
    connect(ui.itemBtn,
            &QToolButton::clicked,
            this,
            [=]()
            {
                auto item = QCtmInputDialog::getItem(this, "Input", "Select item:", QStringList { "Item1", "Item2", "Item3" });
                ui.itemEdit->setText(item);
            });
    connect(ui.intBtn,
            &QToolButton::clicked,
            this,
            [=]()
            {
                auto i = QCtmInputDialog::getInt(this, "Input", "Input number:");
                ui.intEdit->setText(QString::number(i));
            });
    connect(ui.doubleBtn,
            &QToolButton::clicked,
            this,
            [=]()
            {
                auto d = QCtmInputDialog::getDouble(this, "Input", "Input number:");
                ui.doubleEdit->setText(QString::number(d));
            });
    connect(ui.textBtn,
            &QToolButton::clicked,
            this,
            [=]()
            {
                auto text = QCtmInputDialog::getText(this, "Input", "Input text:");
                ui.textEdit->setText(text);
            });
    connect(ui.multiTextBtn,
            &QToolButton::clicked,
            this,
            [=]()
            {
                auto text = QCtmInputDialog::getMultiLineText(this, "Input", "Input text:");
                ui.multiTextEdit->setText(text);
            });
}

InputDialogExample::~InputDialogExample() {}
