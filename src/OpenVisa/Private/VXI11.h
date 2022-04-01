#pragma once

#include "IOBase.h"

namespace OpenVisa
{
class VXI11 : public IOBase
{
public:
    VXI11(Object::Attribute const& attr);
    ~VXI11();
    void connect(const Address<AddressType::VXI11>& address,
                 const std::chrono::milliseconds& openTimeout,
                 const std::chrono::milliseconds& commandTimeout);
    void send(const std::string& buffer) const override;
    std::string readAll() const override;
    std::string read(size_t size) const override;
    void close() noexcept override;
    bool connected() const noexcept override;
    size_t avalible() const noexcept override;

private:
    void getPorts();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
} // namespace OpenVisa
