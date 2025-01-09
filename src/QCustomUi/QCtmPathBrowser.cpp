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
#include "QCtmPathBrowser.h"

#include <QHoverEvent>
#include <QLineEdit>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

struct PathNode
{
    QString text;
    QRect rect;
};

struct QCtmPathBrowser::Impl
{
    QString path;
    std::vector<PathNode> nodes;
    bool readOnly { false };
    bool frame { true };
    QTextOption textOption;
    int spliterWidth { 18 };
    int hoverNode { -1 };
    QLineEdit* editor { nullptr };
};

/*!
    \class      QCtmPathBrowser
    \brief      路径浏览器.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmPathBrowser.h
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmPathBrowser::QCtmPathBrowser(QWidget* parent /*= nullptr*/) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed, QSizePolicy::LineEdit));
    m_impl->textOption.setAlignment(Qt::AlignCenter);
    setAttribute(Qt::WA_Hover);
    setLineEdit(new QLineEdit(this));
}

/*!
    \brief      析构函数.
*/
QCtmPathBrowser::~QCtmPathBrowser() {}

/*!
    \brief      设置显示路径 \a path.
    \sa         path
*/
void QCtmPathBrowser::setPath(QString path)
{
    auto tmp = path.replace("\\", "/");
    if (tmp == m_impl->path)
        return;
    m_impl->path = std::move(tmp);
    generatorNodes();
    emit pathChanged(m_impl->path);
}

/*!
    \brief      返回显示路径.
    \sa         setPath
*/
QString QCtmPathBrowser::path() const { return m_impl->path; }

/*!
    \brief      设置是否只读 \a flag.
    \sa         readOnly
*/
void QCtmPathBrowser::setReadOnly(bool flag) { m_impl->readOnly = flag; }

/*!
    \brief      返回是否只读.
    \sa         setReadOnly
*/
bool QCtmPathBrowser::readOnly() const { return m_impl->readOnly; }

/*!
    \brief      设置自定义的文本编辑器 \a editor.
    \sa         lineEdit
*/
void QCtmPathBrowser::setLineEdit(QLineEdit* editor)
{
    Q_ASSERT(editor);
    if (m_impl->editor)
        m_impl->editor->deleteLater();
    m_impl->editor = editor;
    m_impl->editor->installEventFilter(this);
    m_impl->editor->hide();
    connect(m_impl->editor,
            &QLineEdit::editingFinished,
            this,
            [=]()
            {
                setPath(m_impl->editor->text());
                m_impl->editor->hide();
            });
}

/*!
    \brief      返回文本编辑器.
    \sa         setLineEdit
*/
QLineEdit* QCtmPathBrowser::lineEdit() const { return m_impl->editor; }

/*!
    \reimp
*/
void QCtmPathBrowser::paintEvent(QPaintEvent* event)
{
    QStyleOptionFrame opt;
    initStyleOption(&opt);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_PanelLineEdit, &opt, &p, this);

    QStyleOptionViewItem viewOpt;
    viewOpt.state = QStyle::State_Item | QStyle::State_Children | QStyle::State_Enabled;

    QStyleOptionButton btnOpt;
    btnOpt.state = QStyle::State_MouseOver | QStyle::State_Enabled;
    for (auto it = m_impl->nodes.begin(); it != m_impl->nodes.end(); it++)
    {
        if (std::distance(m_impl->nodes.begin(), it) == m_impl->hoverNode)
        {
            btnOpt.rect = it->rect;
            style()->drawPrimitive(QStyle::PE_PanelButtonBevel, &btnOpt, &p, this);
        }
        p.drawText(it->rect, it->text, m_impl->textOption);
        if (it + 1 != m_impl->nodes.end())
        {
            viewOpt.rect = QRect(it->rect.right(), 0, m_impl->spliterWidth, this->height());
            style()->drawPrimitive(QStyle::PE_IndicatorBranch, &viewOpt, &p, this);
        }
    }
}

/*!
    \reimp
*/
QSize QCtmPathBrowser::sizeHint() const { return minimumSizeHint(); }

/*!
    \reimp
*/
QSize QCtmPathBrowser::minimumSizeHint() const
{
    ensurePolished();
    const auto& fm      = fontMetrics();
    const auto& margins = this->contentsMargins();
    int h               = fm.height() + fm.leading() + margins.top() + margins.bottom();
    QStyleOptionFrame opt;
    initStyleOption(&opt);
    return style()->sizeFromContents(QStyle::CT_LineEdit, &opt, QSize(100, 30), this);
}

/*!
    \reimp
*/
void QCtmPathBrowser::resizeEvent(QResizeEvent* event)
{
    if (event->oldSize().height() != event->size().height())
        generatorNodes();
    if (m_impl->editor->isVisible())
    {
        m_impl->editor->setGeometry(0, 0, event->size().width(), event->size().height());
    }
    QWidget::resizeEvent(event);
}

/*!
    \reimp
*/
void QCtmPathBrowser::mousePressEvent(QMouseEvent* event)
{
    if (m_impl->hoverNode == -1 && !m_impl->readOnly)
    {
        m_impl->editor->setText(m_impl->path);
        m_impl->editor->setGeometry(0, 0, this->width(), this->height());
        m_impl->editor->setVisible(true);
    }
}

/*!
    \reimp
*/
void QCtmPathBrowser::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_impl->hoverNode != -1)
    {
        QString path;
        for (auto it = m_impl->nodes.begin(); it != m_impl->nodes.begin() + m_impl->hoverNode + 1; it++)
        {
            if (!path.isEmpty())
                path.append("/");
            path.append(it->text);
        }
        emit pathClicked(path);
    }
}

/*!
    \reimp
*/
bool QCtmPathBrowser::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_impl->editor)
    {
        if (event->type() == QEvent::FocusOut)
            cancelEditor();
        else if (event->type() == QEvent::Show)
        {
            m_impl->editor->setFocus();
            m_impl->editor->selectAll();
        }
        else if (event->type() == QEvent::KeyPress)
        {
            auto evt = dynamic_cast<QKeyEvent*>(event);
            if (evt && evt->key() == Qt::Key_Escape)
                cancelEditor();
        }
    }
    return QWidget::eventFilter(watched, event);
}

/*!
    \reimp
*/
bool QCtmPathBrowser::event(QEvent* e)
{
    if (e->type() == QEvent::HoverMove)
    {
        if (auto evt = dynamic_cast<QHoverEvent*>(e); evt)
        {
            int hoverNode = -1;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            auto pos = evt->position().toPoint();
#else
            auto pos = evt->pos();
#endif
            if (auto it =
                    std::find_if(m_impl->nodes.begin(), m_impl->nodes.end(), [pos](const auto& node) { return node.rect.contains(pos); });
                it != m_impl->nodes.end())
            {
                hoverNode = std::distance(m_impl->nodes.begin(), it);
            }
            if (m_impl->hoverNode != hoverNode)
            {
                m_impl->hoverNode = hoverNode;
                update();
            }
        }
    }
    if (e->type() == QEvent::Leave)
    {
        if (m_impl->hoverNode != -1)
        {
            m_impl->hoverNode = -1;
            update();
        }
    }
    return QWidget::event(e);
}

void QCtmPathBrowser::generatorNodes()
{
    QStyleOptionFrame opt;
    initStyleOption(&opt);
    auto rect = this->style()->subElementRect(QStyle::SE_FrameContents, &opt, this);
    if (rect.isEmpty())
        rect = QRect(QPoint(0, 0), this->rect().size());

    auto strs = m_impl->path.split('/', Qt::SplitBehaviorFlags::SkipEmptyParts);
    m_impl->nodes.resize(strs.size());
    int x = rect.left();
    for (int i = 0; i < strs.size(); i++)
    {
        if (i != 0)
        {
            x += m_impl->spliterWidth;
        }
        auto width            = this->fontMetrics().horizontalAdvance(strs[i]) + 10;
        m_impl->nodes[i].text = strs[i];
        m_impl->nodes[i].rect = QRect(x, rect.top(), width, rect.height());
        x += width;
    }
}

/*!
    \brief      初始化样式选项 \a option.
*/
void QCtmPathBrowser::initStyleOption(QStyleOptionFrame* option) const
{
    if (!option)
        return;

    option->initFrom(this);
    option->rect         = contentsRect();
    option->lineWidth    = m_impl->frame ? style()->pixelMetric(QStyle::PM_DefaultFrameWidth, option, this) : 0;
    option->midLineWidth = 0;
    option->state |= QStyle::State_Sunken;
    if (m_impl->readOnly)
        option->state |= QStyle::State_ReadOnly;
    option->features = QStyleOptionFrame::None;
}

/*!
    \brief      取消编辑操作.
*/
void QCtmPathBrowser::cancelEditor()
{
    m_impl->editor->setText(m_impl->path);
    m_impl->editor->hide();
}
