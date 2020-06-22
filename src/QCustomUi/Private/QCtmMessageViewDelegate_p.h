#pragma once

#include <QItemDelegate>

#include <memory>

class QCtmMessageViewDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    QCtmMessageViewDelegate(QObject* parent);
    ~QCtmMessageViewDelegate();

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index)const override;

    void setDecoration(const QColor& color);
    const QColor& decoration()const;

    void setCloseButtonIcon(const QPixmap& pixmap);
    const QPixmap& closeButtonIcon() const;
protected:
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;
private:
    void drawTitle(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void drawDateTime(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void drawCloseButton(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void drawDecoration(QPainter* painter, const QStyleOptionViewItem& option, const QRect& rect, const QPixmap& pixmap) const;
    void drawBackground(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index)const;

    QRect doDateTimeRect(const QStyleOptionViewItem& option, const QModelIndex& model) const;
    QRect doTitleRect(const QStyleOptionViewItem& option, const QModelIndex& model) const;
    QRect doCloseBtnRect(const QStyleOptionViewItem& option) const;
signals:
    void closeButtonClicked(const QModelIndex& index);
    void titleClicked(const QModelIndex& index);
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

