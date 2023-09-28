/*********************************************************************************
**                                                                              **
**  Copyright (C) 2022-2023 LiLong                                              **
**  This file is part of OpenVisa.                                              **
**                                                                              **
**  OpenVisa is free software: you can redistribute it and/or modify            **
**  it under the terms of the GNU Lesser General Public License as published by **
**  the Free Software Foundation, either version 3 of the License, or           **
**  (at your option) any later version.                                         **
**                                                                              **
**  OpenVisa is distributed in the hope that it will be useful,                 **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of              **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               **
**  GNU Lesser General Public License for more details.                         **
**                                                                              **
**  You should have received a copy of the GNU Lesser General Public License    **
**  along with OpenVisa.  If not, see <https://www.gnu.org/licenses/>.          **
**********************************************************************************/
#pragma once

#include "Flags.h"
#include "Object.h"
#include "StatusByteRegister.h"
#if __has_include("ADL.h")
#include "ADL.h"
#endif
#if __has_include("Private/ADL.h")
#include "Private/ADL.h"
#endif

#include <concepts>
#include <string>
#include <tuple>
#include <vector>

namespace OpenVisa
{
OPENVISA_EXPORT [[nodiscard]] std::vector<std::string> split(const std::string& source, const std::string& s);
OPENVISA_EXPORT [[nodiscard]] std::string join(const std::vector<std::string>& source, const std::string& delimiter);

template<typename T>
extern std::string encode(const T& arg);

template<typename T>
requires std::ranges::range<T>
[[nodiscard]] inline std::string join(const T& enums, const std::string& delimiter)
{
    std::string ret;
    for (auto it = enums.begin(); it != enums.end(); ++it)
    {
        ret.append(encode(*it));
        if (it + 1 != enums.end())
            ret.append(delimiter);
    }
    return ret;
}

template<typename T>
requires std::is_enum_v<T>
[[nodiscard]] inline std::string join(const std::vector<T>& enums, const std::string& delimiter)
{
    std::string ret;
    for (auto it = enums.begin(); it != enums.end(); ++it)
    {
        ret.append(encode(*it));
        if (it + 1 != enums.end())
            ret.append(delimiter);
    }
    return ret;
}

template<typename... Args>
struct VisaAdl<std::tuple<Args...>>
{
    inline static std::string toScpi(const std::tuple<Args...>& arg)
    {
        static_assert(sizeof(std::tuple<Args...>), "Please define your ADL function: std::string toScpi(YourType).");
    }
    inline static void fromScpi(const std::string& ret, std::tuple<Args...>& out)
    {
        out = _TupleHelper<std::tuple<Args...>>(split(ret, ","));
    }
};

template<>
struct OPENVISA_EXPORT VisaAdl<bool>
{
    static void fromScpi(const std::string& ret, bool& val);
    static std::string toScpi(bool source);
};

template<>
struct OPENVISA_EXPORT VisaAdl<double>
{
    static void fromScpi(const std::string& ret, double& val);
    static std::string toScpi(double source);
};

template<>
struct OPENVISA_EXPORT VisaAdl<long double>
{
    static void fromScpi(const std::string& ret, long double& val);
    static std::string toScpi(long double source);
};

template<>
struct OPENVISA_EXPORT VisaAdl<float>
{
    static void fromScpi(const std::string& ret, float& val);
    static std::string toScpi(float source);
};

template<>
struct OPENVISA_EXPORT VisaAdl<int>
{
    static void fromScpi(const std::string& ret, int& val);
    static std::string toScpi(int source);
};

template<>
struct OPENVISA_EXPORT VisaAdl<unsigned int>
{
    static void fromScpi(const std::string& ret, unsigned int& val);
    static std::string toScpi(unsigned int source);
};

template<>
struct OPENVISA_EXPORT VisaAdl<long>
{
    static void fromScpi(const std::string& ret, long& val);
    static std::string toScpi(long source);
};

template<>
struct OPENVISA_EXPORT VisaAdl<unsigned long>
{
    static void fromScpi(const std::string& ret, unsigned long& val);
    static std::string toScpi(unsigned long source);
};

template<>
struct OPENVISA_EXPORT VisaAdl<long long>
{
    static void fromScpi(const std::string& ret, long long& val);
    static std::string toScpi(long long source);
};

template<>
struct OPENVISA_EXPORT VisaAdl<unsigned long long>
{
    static void fromScpi(const std::string& ret, unsigned long long& val);
    static std::string toScpi(unsigned long long source);
};

template<>
struct OPENVISA_EXPORT VisaAdl<short>
{
    static void fromScpi(const std::string& ret, short& val);
    static std::string toScpi(short source);
};

template<>
struct OPENVISA_EXPORT VisaAdl<unsigned short>
{
    static void fromScpi(const std::string& ret, unsigned short& val);
    static std::string toScpi(unsigned short source);
};

template<>
struct OPENVISA_EXPORT VisaAdl<char>
{
    static void fromScpi(const std::string& ret, char& val);
    static std::string toScpi(char source);
};

template<>
struct OPENVISA_EXPORT VisaAdl<unsigned char>
{
    static void fromScpi(const std::string& ret, unsigned char& val);
    static std::string toScpi(unsigned char source);
};

template<>
struct OPENVISA_EXPORT VisaAdl<StatusByteRegisters>
{
    static void fromScpi(const std::string& ret, StatusByteRegisters& val);
    static std::string toScpi(StatusByteRegisters source);
};

template<typename T>
[[nodiscard]] inline std::string encode(const T& arg)
{
    return VisaAdl<T>::toScpi(arg);
}

template<typename T>
[[nodiscard]] inline T decode(const std::string& ret)
{
    T value;
    VisaAdl<T>::fromScpi(ret, value);
    return value;
}

template<size_t i, size_t size, typename T>
constexpr auto _tupleHelper(const auto& val, const std::vector<std::string>& vec)
{
    using type = typename std::tuple_element<i, T>::type;
    if constexpr (i < size - 1)
        return _tupleHelper<i + 1, size, T>(std::tuple_cat(val, std::make_tuple(decode<type>(vec[i]))), vec);
    else
        return std::tuple_cat(val, std::make_tuple(decode<type>(vec[i])));
}

template<typename T>
struct _TupleHelper : T
{
    _TupleHelper(const std::vector<std::string>& vec)
    {
        constexpr auto size = std::tuple_size_v<T>;
        auto tup            = std::make_tuple(decode<std::tuple_element<0, T>::type>(vec[0]));
        *this               = _tupleHelper<1, size, T>(tup, vec);
    }
    using T::operator=;
};

template<size_t offset, typename Tuple, size_t... I>
constexpr inline auto _skipHelper(const Tuple& t, std::index_sequence<I...>)
{
    return std::make_tuple(std::get<I + offset>(t)...);
}

// 跳过元组的前i项，生成一个新的元组,如{1,2,3}调用skip<1>时，返回{2,3}.
template<size_t i, typename Tuple>
constexpr inline auto skipTuple(const Tuple& t)
{
    constexpr auto size = std::tuple_size_v<Tuple>;
    return _skipHelper<i>(t, std::make_index_sequence<size - i>());
}

template<typename T>
struct SampleValue
{
    T value;
    inline operator T() const { return value; }
    inline SampleValue<T>& operator=(const T& val)
    {
        value = val;
        return *this;
    }
};
} // namespace OpenVisa

#define VISA_SAMPLE_VALUE(SubClass, ValueType)                                                                                             \
    struct SubClass : public OpenVisa::SampleValue<ValueType>                                                                              \
    {                                                                                                                                      \
        using SampleValue<ValueType>::operator ValueType;                                                                                  \
        using SampleValue<ValueType>::operator=;                                                                                           \
    }
