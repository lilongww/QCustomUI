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

void OpenVisaConfig::loadConfig() {}

OpenVisaConfig::OpenVisaConfig() {}

OpenVisaConfig::~OpenVisaConfig() {}

} // namespace OpenVisa