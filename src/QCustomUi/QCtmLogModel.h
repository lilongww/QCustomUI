#pragma once

#include "QCtmAbstractLogModel.h"
#include "QCtmLogEvent.h"

#include <QDateTime>
#include <QIcon>

#include <memory>

struct QCUSTOMUI_EXPORT QCtmLogMessage
{
    QString msg;
    QDateTime dateTime;
    QtMsgType type;
};

class QCUSTOMUI_EXPORT QCtmLogModel : public QCtmAbstractLogModel
{
    Q_OBJECT
        Q_PROPERTY(QIcon infoIcon READ infoIcon WRITE setInfoIcon)
        Q_PROPERTY(QIcon warningIcon READ warningIcon WRITE setWarningIcon)
        Q_PROPERTY(QIcon errorIcon READ errorIcon WRITE setErrorIcon)
public:
    enum { TypeRole = Qt::UserRole + 1 };

    QCtmLogModel(const QString& objectName, QObject *parent);
    ~QCtmLogModel();

    void clear() override;
    QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	/**
	 * @brief		设置最大显示消息数量，默认1万条
	 * @param[in]	count 最大显示消息数量
	 */
	void setMaximumCount(int count);

	/**
	 * @brief		最大显示消息数量
	 */
	int maximumCount() const;

    /**
     * @brief		设置日志插入顺序
     * @param[in]	mode 插入顺序
     */
    void setLogInsertMode(LogInsertMode mode);

    /**
     * @brief		获取日志插入顺序
     * @Return:		日志插入顺序
     */
    LogInsertMode logInsertMode() const;

    void setInfoIcon(const QIcon& icon);
    const QIcon& infoIcon() const;

    void setWarningIcon(const QIcon& icon);
    const QIcon& warningIcon() const;

    void setErrorIcon(const QIcon& icon);
    const QIcon& errorIcon() const;

    int warningCount() const;
    int infoCount() const;
    int errorCount() const;
protected:
    void logEvent(QCtmLogEvent* e) override;
    void retranslateUi() override;
private:
    struct Impl;
    std::shared_ptr<Impl> m_impl;
};
