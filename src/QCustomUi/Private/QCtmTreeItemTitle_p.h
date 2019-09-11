#pragma once

#include <QPushButton>
#include <QIcon>

#include <memory>

class QCtmTreeItem;

class QCtmTreeItemTitle : public QPushButton
{
	Q_OBJECT
		Q_PROPERTY(QString text READ text WRITE setText)
public:
	QCtmTreeItemTitle(QCtmTreeItem *parent);
	~QCtmTreeItemTitle();

	void setExpand(bool expand);
	bool isExpand() const;

    void insertAction(int index, QAction* action);
    QAction* actionAt(int index) const;
    int indexOf(QAction* action) const;

protected:
	void paintEvent(QPaintEvent *event) override;
    void actionEvent(QActionEvent *event) override;

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
