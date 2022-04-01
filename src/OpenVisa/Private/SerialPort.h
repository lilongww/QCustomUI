#pragma once

#include "IOBase.h"

#include <memory>

namespace OpenVisa
{
class SerialPort : public IOBase
{
public:
    SerialPort(Object::Attribute const& attr);
    ~SerialPort() noexcept;

    void connect(const Address<AddressType::SerialPort>& addr,
                 const std::chrono::milliseconds& openTimeout,
                 const std::chrono::milliseconds& commandTimeout);
    void send(const std::string& buffer) const override;
    std::string readAll() const override;
    std::string read(size_t size) const override;
    void close() noexcept override;
    bool connected() const noexcept override;
    size_t avalible() const noexcept override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
} // namespace OpenVisa
