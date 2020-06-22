#pragma once

#include <QToolButton>

#include <memory>

class QCtmToolButton : public QToolButton
{
    Q_OBJECT
        Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
        Q_PROPERTY(bool showToolTips READ showToolTips WRITE setShowToolTips)
public:
    QCtmToolButton(QWidget* parent);
    ~QCtmToolButton();

    void setIcon(const QIcon& icon);
    void setShowToolTips(bool show);
    bool showToolTips() const;

protected:
    bool event(QEvent* e) override;
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
