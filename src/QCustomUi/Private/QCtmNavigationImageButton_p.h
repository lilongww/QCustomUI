#pragma once

#include "qcustomui_global.h"
#include "QCtmToolButton_p.h"

#include <memory>


class QCtmNavigationImageButton : public QCtmToolButton
{
    Q_OBJECT

public:
    QCtmNavigationImageButton(QWidget* parent);
    ~QCtmNavigationImageButton();

protected:
    void paintEvent(QPaintEvent* e)override;
    QSize sizeHint()const override;
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
