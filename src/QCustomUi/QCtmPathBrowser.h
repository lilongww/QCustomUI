/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2020 LiLong                                              **
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

#include "qcustomui_global.h"

#include <QWidget>

#include <memory>

class QStyleOptionFrame;

class QCUSTOMUI_EXPORT QCtmPathBrowser : public QWidget
{
    Q_OBJECT
public:
    QCtmPathBrowser(QWidget* parent = nullptr);
    ~QCtmPathBrowser();

    void setPath(const QString& path);
    QString path() const;
    void setReadOnly(bool flag);
    bool readOnly() const;
signals:
    void pathChanged(const QString& path);
    void pathClicked(const QString& path);
protected:
    void paintEvent(QPaintEvent* event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    bool event(QEvent* e) override;
    void resizeEvent(QResizeEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
private:
    void generatorNodes();
    void initStyleOption(QStyleOptionFrame* option) const;
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
