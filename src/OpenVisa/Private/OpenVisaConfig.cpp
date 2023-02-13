#include "OpenVisaConfig.h"
#include "../SerialPortInfo.h"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <cstdlib>
#include <filesystem>
#include <format>
#include <ranges>

constexpr std::string_view ASRL_SECTION = "ASRL-RSRC-ALIAS";
constexpr std::string_view FILENAME     = "OpenVisaConfig.ini";

namespace OpenVisa
{
struct OpenVisaConfig::Impl
{
    std::map<unsigned int, Asrl> asrlMap;
#ifdef WIN32
    const std::string path { std::format("{}/OpenVisa", std::getenv("ALLUSERSPROFILE")) };
#else
    const std::string path { "/etc/OpenVisa" };
#endif
};

OpenVisaConfig& OpenVisaConfig::instance()
{
    static OpenVisaConfig inst;
    return inst;
}

const std::map<unsigned int, Asrl>& OpenVisaConfig::asrlMap() const { return m_impl->asrlMap; }

std::optional<int> OpenVisaConfig::toAsrl(const std::string& portName) const
{
    auto ports = SerialPortInfo::listPorts();
    if (std::ranges::find_if(ports, [&](const auto& port) { return port.portName() == portName; }) != ports.end())
    {
        auto reFind = [&]()
        {
            return std::find_if(m_impl->asrlMap.begin(),
                                m_impl->asrlMap.end(),
                                [&](const std::pair<unsigned int, Asrl>& kv) { return kv.second.portName == portName; });
        };
        auto it = reFind();
        if (it != m_impl->asrlMap.end())
            return it->first;

        // reload config
        const_cast<OpenVisaConfig*>(this)->loadConfig();
        it = reFind();
        if (it != m_impl->asrlMap.end())
            return it->first;

        // update asrl list
        const_cast<OpenVisaConfig*>(this)->updateAsrl();
        saveConfig();
        it = reFind();
        if (it != m_impl->asrlMap.end())
            return it->first;
    }
    return std::nullopt;
}

std::optional<Asrl> OpenVisaConfig::fromAsrl(unsigned int asrl) const
{
    auto it = m_impl->asrlMap.find(asrl);
    if (it != m_impl->asrlMap.end())
        return it->second;

    // reload config
    const_cast<OpenVisaConfig*>(this)->loadConfig();
    it = m_impl->asrlMap.find(asrl);
    if (it != m_impl->asrlMap.end())
        return it->second;

    // update asrl list
    const_cast<OpenVisaConfig*>(this)->updateAsrl();
    saveConfig();

    it = m_impl->asrlMap.find(asrl);
    return it == m_impl->asrlMap.end() ? std::optional<Asrl> {} : it->second;
}

void OpenVisaConfig::loadConfig()
{
    auto file = std::format("{}/{}", m_impl->path, FILENAME);
    if (!std::filesystem::exists(file))
    {
        saveDefault();
    }
    else
    {
        boost::property_tree::ptree root;
        try
        {
            boost::property_tree::read_ini(file, root);
            auto asrlNode = root.get_child(ASRL_SECTION.data());
            if (!asrlNode.empty())
            {
                auto size = asrlNode.get<int>("NumOfResources");
                for (auto i : std::views::iota(0, size))
                {
                    auto name = asrlNode.get<std::string>(std::format("Name{}", i));
                    std::string temp;
                    std::ranges::copy(name | std::views::filter([=](auto ch) { return ch >= '0' && ch <= '9'; }), std::back_inserter(temp));
                    m_impl->asrlMap.insert(
                        std::pair(static_cast<unsigned int>(std::stoul(temp)),
                                  Asrl { asrlNode.get<std::string>(std::format("SystemName{}", i)),
                                         asrlNode.get<unsigned int>(std::format("BaudRate{}", i)),
                                         static_cast<DataBits>(asrlNode.get<int>(std::format("DataBits{}", i))),
                                         static_cast<FlowControl>(asrlNode.get<int>(std::format("FlowCtrl{}", i))),
                                         static_cast<Parity>(asrlNode.get<int>(std::format("Parity{}", i))),
                                         static_cast<StopBits>((asrlNode.get<int>(std::format("StopBits{}", i)) - 10) / 5) }));
                }
            }
        }
        catch (const std::exception&)
        {
            saveDefault();
        }
    }
}

void OpenVisaConfig::saveConfig() const
{
    boost::property_tree::ptree root;
    auto k  = [](std::string_view name) -> std::string { return std::format("{}.{}", ASRL_SECTION, name); };
    auto it = m_impl->asrlMap.begin();
    for (auto i : std::views::iota(0ULL, m_impl->asrlMap.size()))
    {
        root.put(k(std::format("Name{}", i)), std::format("ASRL{}::INSTR", it->first));
        root.put(k(std::format("Enabled{}", i)), 1);
        root.put(k(std::format("Static{}", i)), 0);
        root.put(k(std::format("SystemName{}", i)), it->second.portName);
        root.put(k(std::format("BaudRate{}", i)), it->second.baud);
        root.put(k(std::format("Parity{}", i)), static_cast<int>(it->second.parity));
        root.put(k(std::format("StopBits{}", i)), static_cast<int>(it->second.stopBits) * 5 + 10);
        root.put(k(std::format("DataBits{}", i)), static_cast<int>(it->second.dataBits));
        root.put(k(std::format("FlowCtrl{}", i)), static_cast<int>(it->second.flowControl));
        it++;
    }
    root.put(k("NumOfResources"), m_impl->asrlMap.size());
    try
    {
        if (!std::filesystem::exists(m_impl->path))
        {
            std::filesystem::create_directories(m_impl->path);
        }
        boost::property_tree::write_ini(std::format("{}/{}", m_impl->path, FILENAME), root);
    }
    catch (const std::exception&)
    {
    }
}

void OpenVisaConfig::updateAsrl()
{
    auto ports = SerialPortInfo::listPorts();
    for (const auto& port : ports)
    {

        if (std::find_if(m_impl->asrlMap.begin(),
                         m_impl->asrlMap.end(),
                         [&](const std::pair<unsigned int, Asrl>& kv)
                         { return kv.second.portName == port.portName(); }) == m_impl->asrlMap.end())
        {
            addAsrl(Asrl { port.portName() });
        }
    }
}

void OpenVisaConfig::addAsrl(const Asrl& addr)
{
    unsigned int asrl = 0;
    std::string numStr;
    std::ranges::copy(addr.portName | std::views::filter([=](char c) { return c >= '0' && c <= '9'; }), std::back_inserter(numStr));
    try
    {
        asrl = static_cast<unsigned int>(std::stoul(numStr));
    }
    catch (const std::exception&)
    {
    }
    if (asrl == 0 || m_impl->asrlMap.find(asrl) != m_impl->asrlMap.end())
        asrl = m_impl->asrlMap.rbegin()->first + 1;
    m_impl->asrlMap.insert(std::pair(asrl, addr));
}

void OpenVisaConfig::saveDefault()
{
    updateAsrl();
    saveConfig();
}

OpenVisaConfig::OpenVisaConfig() : m_impl(std::make_unique<Impl>()) {}

OpenVisaConfig::~OpenVisaConfig() {}

} // namespace OpenVisa
