#include "QCtmDockSplitter_p.h"

QCtmDockSplitter::~QCtmDockSplitter()
{
}

bool QCtmDockSplitter::contains(QWidget* widget)
{
    for (int i = 0; i < count(); i++)
    {
        if (widget == this->widget(i))
            return true;
    }
    return false;
}
