/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2025 LiLong                                              **
**  This file is part of QCustomUi.                                             **
**                                                                              **
**  QCustomUi is free software: you can redistribute it and/or modify           **
**  it under the terms of the GNU Lesser General Public License as published by **
**  the Free Software Foundation, either version 3 of the License, or           **
**  (at your option) any later version.                                         **
**                                                                              **
**  QCustomUi is distributed in the hope that it will be useful,                **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of              **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               **
**  GNU Lesser General Public License for more details.                         **
**                                                                              **
**  You should have received a copy of the GNU Lesser General Public License    **
**  along with QCustomUi.  If not, see <https://www.gnu.org/licenses/>.         **
**********************************************************************************/
#pragma once

#include "qcustomui_global.h"

#include <QLineEdit>
#include <QWidget>

#include <memory>

class QStyleOptionFrame;

class QCUSTOMUI_EXPORT QCtmPathBrowser : public QWidget
{
    Q_PROPERTY(QString path READ path WRITE setPath)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly)
    Q_OBJECT
public:
    explicit QCtmPathBrowser(QWidget* parent = nullptr);
    ~QCtmPathBrowser();

    void setPath(QString path);
    QString path() const;
    void setReadOnly(bool flag);
    bool readOnly() const;
    void setLineEdit(QLineEdit* editor);
    QLineEdit* lineEdit() const;
signals:
    void pathChanged(const QString& path);
    void pathClicked(const QString& path);

protected:
    void paintEvent(QPaintEvent* event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    bool event(QEvent* e) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
    virtual void initStyleOption(QStyleOptionFrame* option) const;
    void cancelEditor();

private:
    void generatorNodes();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
