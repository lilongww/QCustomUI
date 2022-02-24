#include "PathBrowserExample.h"

#include <QCustomUi/QCtmMessageBox.h>
#include <QCustomUi/QCtmPathBrowser.h>

#include <QVBoxLayout>

PathBrowserExample::PathBrowserExample(QWidget* parent /*= nullptr*/) : QCtmWindow(parent)
{
    auto layout = new QVBoxLayout(this->centralWidget());
    layout->addWidget(m_path1 = new QCtmPathBrowser(this));
    layout->addWidget(m_path2 = new QCtmPathBrowser(this));

    m_path1->setPath("C:/Windows/System32");
    m_path2->setPath("C:/Windows/System32");
    m_path2->setReadOnly(true);

    connect(m_path1,
            &QCtmPathBrowser::pathClicked,
            this,
            [=](const QString& path) { QCtmMessageBox::information(this, tr("Path Clicked"), path); });
    connect(m_path1,
            &QCtmPathBrowser::pathChanged,
            this,
            [=](const QString& path) { QCtmMessageBox::information(this, tr("Path Changed"), path); });
    connect(m_path2,
            &QCtmPathBrowser::pathClicked,
            this,
            [=](const QString& path) { QCtmMessageBox::information(this, tr("Path Clicked"), path); });
    connect(m_path2,
            &QCtmPathBrowser::pathChanged,
            this,
            [=](const QString& path) { QCtmMessageBox::information(this, tr("Path Changed"), path); });
}

PathBrowserExample::~PathBrowserExample() {}
