#include "PathBrowserExample.h"

#include <QCustomUi/QCtmPathBrowser.h>
#include <QCustomUi/QCtmMessageBox.h>

#include <QHBoxLayout>

PathBrowserExample::PathBrowserExample(QWidget* parent /*= nullptr*/)
    : QCtmWindow(parent)
    , m_path(new QCtmPathBrowser(this))
{
    auto layout = new QHBoxLayout(this->centralWidget());
    layout->addWidget(m_path);
    m_path->setPath("C:/Windows/System32");
    connect(m_path, &QCtmPathBrowser::pathClicked, this, [=](const QString& path)
        {
            QCtmMessageBox::information(this, tr("Tips"), path);
        });
}

PathBrowserExample::~PathBrowserExample()
{

}
