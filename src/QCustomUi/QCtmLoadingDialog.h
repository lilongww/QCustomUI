#pragma once

#include "qcustomui_global.h"

#include <QDialog>

class QCUSTOMUI_EXPORT QCtmLoadingDialog : public QDialog
{
    Q_OBJECT

public:
    QCtmLoadingDialog(QWidget* parent = nullptr);
    ~QCtmLoadingDialog();

protected:
    void showEvent(QShowEvent*) override;
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
