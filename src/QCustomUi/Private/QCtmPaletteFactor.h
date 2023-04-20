#pragma once

#include <QWidget>

#include <memory>

class QCtmPaletteFactor
{
public:
    QCtmPaletteFactor();
    static void init(QWidget* widget);

protected:
    virtual void initQCtmTitleBar(QWidget* bar);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};