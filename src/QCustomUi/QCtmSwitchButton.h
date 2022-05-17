#pragma once

#include "qcustomui_global.h"

#include <QAbstractButton>

class QCUSTOMUI_EXPORT QCtmSwitchButton : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(QBrush uncheckedBackground READ uncheckedBackground WRITE setUncheckedBackground)
    Q_PROPERTY(QColor uncheckedBackgroundBorderColor READ uncheckedBackgroundBorderColor WRITE setUncheckedBackgroundBorderColor)
    Q_PROPERTY(QBrush uncheckedHandleBrush READ uncheckedHandleBrush WRITE setUncheckedHandleBrush)
    Q_PROPERTY(QColor uncheckedHandleBorderColor READ uncheckedHandleBorderColor WRITE setUncheckedHandleBorderColor)
    Q_PROPERTY(QBrush checkedBackground READ checkedBackground WRITE setCheckedBackground)
    Q_PROPERTY(QColor checkedBackgroundBorderColor READ checkedBackgroundBorderColor WRITE setCheckedBackgroundBorderColor)
    Q_PROPERTY(QBrush checkedHandleBrush READ checkedHandleBrush WRITE setCheckedHandleBrush)
    Q_PROPERTY(QColor checkedHandleBorderColor READ checkedHandleBorderColor WRITE setCheckedHandleBorderColor)
public:
    QCtmSwitchButton(QWidget* parent);
    ~QCtmSwitchButton();

    void setUncheckedBackground(const QBrush& brush);
    const QBrush& uncheckedBackground() const;
    void setUncheckedBackgroundBorderColor(const QColor& pen);
    const QColor& uncheckedBackgroundBorderColor() const;
    void setUncheckedHandleBrush(const QBrush& brush);
    const QBrush& uncheckedHandleBrush() const;
    void setUncheckedHandleBorderColor(const QColor& pen);
    const QColor& uncheckedHandleBorderColor() const;
    void setCheckedBackground(const QBrush& brush);
    const QBrush& checkedBackground() const;
    void setCheckedBackgroundBorderColor(const QColor& pen);
    const QColor& checkedBackgroundBorderColor() const;
    void setCheckedHandleBrush(const QBrush& brush);
    const QBrush& checkedHandleBrush() const;
    void setCheckedHandleBorderColor(const QColor& pen);
    const QColor& checkedHandleBorderColor() const;

protected:
    void paintEvent(QPaintEvent* e) override;
    void resizeEvent(QResizeEvent* event) override;
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
