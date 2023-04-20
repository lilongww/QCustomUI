#include "QCtmPaletteFactor.h"
#include "../QCtmTitleBar.h"
#include "QCtmMaximumSizeButton_p.h"

#include <functional>
#include <map>

struct QCtmPaletteFactor::Impl
{
    std::map<QString, std::function<void(QWidget*)>> funcs;
};

QCtmPaletteFactor::QCtmPaletteFactor() : m_impl(std::make_unique<Impl>())
{
    auto& funcs = m_impl->funcs;
    funcs.insert({ QCtmTitleBar::staticMetaObject.className(), std::bind_front(&QCtmPaletteFactor::initQCtmTitleBar, this) });
}

void QCtmPaletteFactor::init(QWidget* widget)
{
    static QCtmPaletteFactor ins;
    if (auto it = ins.m_impl->funcs.find(widget->metaObject()->className()); it != ins.m_impl->funcs.end())
    {
        std::invoke(it->second, widget);
    }
}

void QCtmPaletteFactor::initQCtmTitleBar(QWidget* bar)
{
    auto palette = bar->palette();
    palette.setBrush(QPalette::Window, Qt::white);
    bar->setPalette(palette);
    if (auto minBtn = bar->findChild<QWidget*>("minimumSizeBtn"))
    {
        minBtn->setProperty("icon", QIcon(":/QCustomUi/Resources/minimize-light.png"));
        auto palette = minBtn->palette();
        palette.setBrush(QPalette::Button, Qt::NoBrush);

        minBtn->setPalette(palette);
    }
    if (auto maxBtn = bar->findChild<QCtmMaximumSizeButton*>("maximumSizeBtn"))
    {
        maxBtn->setIcon(QIcon(":/QCustomUi/Resources/maximize_max-light.png"));
        QObject::connect(
            maxBtn,
            &QCtmMaximumSizeButton::maximumSizedChanged,
            maxBtn,
            [=](bool max)
            { maxBtn->setIcon(QIcon(max ? ":/QCustomUi/Resources/maximize-light.png" : ":/QCustomUi/Resources/maximize_max-light.png")); });
    }
    if (auto minBtn = bar->findChild<QWidget*>("closeBtn"))
    {
        minBtn->setProperty("icon", QIcon(":/QCustomUi/Resources/close-light.png"));
    }
}
