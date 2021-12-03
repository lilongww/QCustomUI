#include "QCtmPathBrowser.h"

#include <QPainter>
#include <QStyleOption>
#include <QStyle>
#include <QHoverEvent>

struct PathNode
{
    QString text;
    QRect rect;
};

struct QCtmPathBrowser::Impl
{
    QString path;
    std::vector<PathNode> nodes;
    bool readOnly{ false };
    bool frame{ true };
    QTextOption textOption;
    int spliterWidth{ 18 };
    int hoverNode{ -1 };
};

QCtmPathBrowser::QCtmPathBrowser(QWidget* parent /*= nullptr*/)
    : QWidget(parent)
    , m_impl(std::make_unique<Impl>())
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed, QSizePolicy::LineEdit));
    m_impl->textOption.setAlignment(Qt::AlignCenter);
    setAttribute(Qt::WA_Hover);
}

QCtmPathBrowser::~QCtmPathBrowser()
{

}

void QCtmPathBrowser::setPath(const QString& path)
{
    (m_impl->path = path).replace("\\", "/");
    generatorNodes();
    emit pathChanged(m_impl->path);
}

QString QCtmPathBrowser::path() const
{
    return m_impl->path;
}

void QCtmPathBrowser::setReadOnly(bool flag)
{
    m_impl->readOnly = flag;
}

bool QCtmPathBrowser::readOnly() const
{
    return m_impl->readOnly;
}

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

QSize QCtmPathBrowser::sizeHint() const
{
    return minimumSizeHint();
}

QSize QCtmPathBrowser::minimumSizeHint() const
{
    ensurePolished();
    const auto& fm = fontMetrics();
    const auto& margins = this->contentsMargins();
    int h = fm.height() + fm.leading() + margins.top() + margins.bottom();
    QStyleOptionFrame opt;
    initStyleOption(&opt);
    return style()->sizeFromContents(QStyle::CT_LineEdit, &opt, QSize(100, 30), this);
}

void QCtmPathBrowser::resizeEvent(QResizeEvent* event)
{
    if (event->oldSize().height() != event->size().height())
        generatorNodes();
    QWidget::resizeEvent(event);
}

void QCtmPathBrowser::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_impl->hoverNode != -1)
    {
        QString path;
        for (auto it = m_impl->nodes.begin(); it != m_impl->nodes.begin() + m_impl->hoverNode + 1; it++)
        {
            path.append(it->text);
            if (it + 1 != m_impl->nodes.end())
                path.append("/");
        }
        emit pathClicked(path);
    }
}

bool QCtmPathBrowser::event(QEvent* e)
{
    if (e->type() == QEvent::HoverMove)
    {
        if (auto evt = dynamic_cast<QHoverEvent*>(e); evt)
        {
            int hoverNode = -1;
            auto pos = evt->position().toPoint();
            if (auto it = std::find_if(m_impl->nodes.begin(), m_impl->nodes.end(), [pos](const auto& node)
                {
                    return node.rect.contains(pos);
                }); it != m_impl->nodes.end())
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
        auto width = this->fontMetrics().horizontalAdvance(strs[i]) + 10;
        m_impl->nodes[i].text = strs[i];
        m_impl->nodes[i].rect = QRect(x, rect.top(), width, rect.height());
        x += width;
    }
}

void QCtmPathBrowser::initStyleOption(QStyleOptionFrame* option) const
{
    if (!option)
        return;

    option->initFrom(this);
    option->rect = contentsRect();
    option->lineWidth = m_impl->frame ? style()->pixelMetric(QStyle::PM_DefaultFrameWidth, option, this)
        : 0;
    option->midLineWidth = 0;
    option->state |= QStyle::State_Sunken;
    if (m_impl->readOnly)
        option->state |= QStyle::State_ReadOnly;
    option->features = QStyleOptionFrame::None;
}
