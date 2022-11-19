#include "ToolBoxExample.h"
#include "TestDialog.h"

#include <QCustomUi/QCtmMessageBox.h>

#include <QAction>
#include <QIcon>

struct ToolBoxExample::Impl
{
    TestDialog* dialog { nullptr };
};

ToolBoxExample::ToolBoxExample(QWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    ui.setupUi(this);
    QIcon icon;
    icon.addFile(":/ToolBoxExample/Resource/NewFolder.svg", QSize(), QIcon::Mode::Normal);
    icon.addFile(":/ToolBoxExample/Resource/NewFolderActive.svg", QSize(), QIcon::Mode::Selected);
    auto action = new QAction(icon, "", this);
    addAction(action);
    connect(action, &QAction::triggered, this, [=]() { QCtmMessageBox::information(this, tr("Tips"), tr("New Folder")); });
    // TEST DIALOG BUG
    m_impl->dialog = new TestDialog(this);
    connect(ui.toolButton, &QToolButton::clicked, this, [=]() { m_impl->dialog->exec(); });
    // end
}

ToolBoxExample::~ToolBoxExample() {}
