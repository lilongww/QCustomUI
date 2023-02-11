#include "OpenVisaConfig.h"
#include "../Address.h"

namespace OpenVisa
{
struct OpenVisaConfig::Impl
{
    std::map<unsigned int, Address<AddressType::SerialPort>> asrlMap;
};

OpenVisaConfig& OpenVisaConfig::instance()
{
    static OpenVisaConfig inst;
    return inst;
}

const std::map<unsigned int, OpenVisa::Address<OpenVisa::AddressType::SerialPort>>& OpenVisaConfig::asrlMap() const
{
    return m_impl->asrlMap;
}

std::optional<int> OpenVisaConfig::toAsrl(const Address<AddressType::SerialPort>& addr) const
{
    auto it = std::find_if(m_impl->asrlMap.begin(),
                           m_impl->asrlMap.end(),
                           [&](const std::pair<unsigned int, Address<AddressType::SerialPort>>& kv)
                           { return kv.second.portName() == addr.portName(); });
    return it == m_impl->asrlMap.end() ? std::optional<int> {} : it->first;
}

std::optional<OpenVisa::Address<OpenVisa::AddressType::SerialPort>> OpenVisaConfig::fromAsrl(unsigned int asrl) const
{
    auto it = m_impl->asrlMap.find(asrl);
    return it == m_impl->asrlMap.end() ? std::optional<OpenVisa::Address<OpenVisa::AddressType::SerialPort>> {} : it->second;
}

void OpenVisaConfig::loadConfig() {}

OpenVisaConfig::OpenVisaConfig() {}

OpenVisaConfig::~OpenVisaConfig() {}

} // namespace OpenVisa
