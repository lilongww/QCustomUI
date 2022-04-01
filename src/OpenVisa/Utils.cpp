#include "Utils.h"

#include <regex>

namespace OpenVisa
{
void VisaAdl<bool>::fromScpi(const std::string& ret, bool& val)
{
    auto r = ret;
    if (r.ends_with("\n"))
    {
        r.erase(r.end() - 1);
    }
    else if (r.ends_with("\r\n"))
    {
        r.erase(r.end() - 2);
    }
    else if (r.ends_with("\r"))
    {
        r.erase(r.end() - 2);
    }
    if (r == "ON")
        val = true;
    else if (r == "OFF")
        val = false;
    else if (r == "1" || r == "+1")
        val = true;
    else if (r == "0")
        val = false;
    else
        throw std::exception((std::string("Visa::parseBool Unknown ret:") + r).c_str());
}

void VisaAdl<double>::fromScpi(const std::string& ret, double& val)
{
    auto r = ret;
    if (r.ends_with("\n"))
    {
        r.erase(r.end() - 1);
    }
    else if (r.ends_with("\r\n"))
    {
        r.erase(r.end() - 2);
    }
    else if (r.ends_with("\r"))
    {
        r.erase(r.end() - 2);
    }
    val = std::stod(ret);
}

std::string VisaAdl<double>::toScpi(double val) { return std::to_string(val); }

void VisaAdl<int>::fromScpi(const std::string& ret, int& val)
{
    auto r = ret;
    if (r.ends_with("\n"))
    {
        r.erase(r.end() - 1);
    }
    else if (r.ends_with("\r\n"))
    {
        r.erase(r.end() - 2);
    }
    else if (r.ends_with("\r"))
    {
        r.erase(r.end() - 2);
    }
    val = std::stoi(ret);
}

std::string VisaAdl<int>::toScpi(int val) { return std::to_string(val); }

std::string VisaAdl<bool>::toScpi(bool source) { return source ? "ON" : "OFF"; }

std::vector<std::string> split(const std::string& source, const std::string& s)
{
    std::regex re { s };
    return std::vector<std::string> { std::sregex_token_iterator(source.begin(), source.end(), re, -1), std::sregex_token_iterator() };
}
} // namespace OpenVisa
