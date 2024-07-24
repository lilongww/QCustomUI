#include "QCtmSerialPortComboBox.h"

#include <QSerialPortInfo>

struct QCtmSerialPortComboBox::Impl
{
    QStringList items;
};

/*!
    \class      QCtmSerialPortComboBox
    \brief      串口选择框，下拉选择时自动刷新串口列表.
    \inherits   QCtmComboBox
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmSerialPortComboBox.h
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmSerialPortComboBox::QCtmSerialPortComboBox(QWidget* parent /*= nullptr*/) : QCtmComboBox(parent), m_impl(std::make_unique<Impl>())
{
    connect(this, &QCtmComboBox::aboutToShowPopup, this, &QCtmSerialPortComboBox::onAboutToShowPopup);
    onAboutToShowPopup();
}

/*!
    \brief      析构函数.
*/
QCtmSerialPortComboBox::~QCtmSerialPortComboBox() {}

/*!
    \brief      响应弹出框消息，刷新串口列表.
*/
void QCtmSerialPortComboBox::onAboutToShowPopup()
{
    auto oldItem = this->currentText();
    auto ports   = QSerialPortInfo::availablePorts();
    QStringList items;
    for (const auto& port : ports)
    {
        items.push_back(port.portName());
    }
    std::ranges::sort(items);
    if (m_impl->items == items)
        return;
    m_impl->items = items;

    QSignalBlocker blocker(this);
    clear();
    addItems(items);
    if (items.contains(oldItem))
    {
        setCurrentText(oldItem);
    }
    else
    {
        blocker.unblock();
        auto current = currentText();
        if (this->isEditable())
            emit editTextChanged(current);
        emit currentTextChanged(current);
        emit currentIndexChanged(currentIndex());
    }
}
