#pragma once

#include <QCustomUi/QCtmWindow.h>

class QCtmPathBrowser;
class PathBrowserExample : public QCtmWindow
{
    Q_OBJECT
public:
    PathBrowserExample(QWidget* parent = nullptr);
    ~PathBrowserExample();

private:
    QCtmPathBrowser* m_path1;
    QCtmPathBrowser* m_path2;
};
