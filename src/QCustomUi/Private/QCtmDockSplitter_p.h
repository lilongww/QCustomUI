#pragma once

#include <QSplitter>

class QCtmDockSplitter : public QSplitter
{
    Q_OBJECT

public:
    using QSplitter::QSplitter;
    ~QCtmDockSplitter();

    bool contains(QWidget* widget);
};
