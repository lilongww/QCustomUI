﻿#pragma once

#include "../Address.h"
#include "IOBase.h"

#include <chrono>
#include <memory>

namespace OpenVisa
{
class RawSocket : public IOBase
{
public:
    RawSocket(Object::Attribute const& attr);
    ~RawSocket() noexcept;

    void connect(const Address<AddressType::RawSocket>& addr,
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