#include "ToolBoxExample.h"

#include <QCustomUi/QCtmMessageBox.h>

#include <QAction>
#include <QIcon>

ToolBoxExample::ToolBoxExample(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    QIcon icon;
    icon.addFile(":/ToolBoxExample/Resource/NewFolder.svg", QSize(), QIcon::Mode::Normal);
    icon.addFile(":/ToolBoxExample/Resource/NewFolderActive.svg", QSize(), QIcon::Mode::Selected);
    auto action = new QAction(icon, "", this);
    addAction(action);
    connect(action, &QAction::triggered, this, [=]()
        {
            QCtmMessageBox::information(this, tr("Tips"), tr("New Folder"));
        });
}

ToolBoxExample::~ToolBoxExample()
{

}