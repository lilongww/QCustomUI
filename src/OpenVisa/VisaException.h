#pragma once

#include <exception>

namespace OpenVisa
{
class Exception : public std::exception
{
public:
    inline Exception(int code, const char* what) : std::exception(what), m_code(code) {}
    int code() const { return m_code; }

private:
    int m_code { 0 };
};
} // namespace OpenVisa
