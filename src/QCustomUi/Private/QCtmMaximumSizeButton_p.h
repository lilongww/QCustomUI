#pragma once

#include <QPushButton>

class QCtmMaximumSizeButton : public QPushButton
{
    Q_OBJECT
        Q_PROPERTY(bool maximumsized READ maximumSized WRITE setMaximumSized)
public:
    QCtmMaximumSizeButton(QWidget* parent);
    ~QCtmMaximumSizeButton();

    void setMaximumSized(bool max);
    bool maximumSized()const;

private:
    bool m_maxsized;
};
