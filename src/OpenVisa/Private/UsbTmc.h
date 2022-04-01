#pragma once

#include "IOBase.h"

namespace OpenVisa
{
class UsbTmc : public IOBase
{
public:
    UsbTmc(Object::Attribute const& attr);
    ~UsbTmc();

    void connect(const Address<AddressType::USB>& addr,
                 const std::chrono::milliseconds& openTimeout,
                 const std::chrono::milliseconds& commandTimeout);
    void send(const std::string& buffer) const override;
    std::string readAll() const override;
    std::string read(size_t size) const override;
    void close() noexcept override;
    bool connected() const noexcept override;
    //此处 avalible 必须先调用read方法
    size_t avalible() const noexcept override;

private:
    void init();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
} // namespace OpenVisa
