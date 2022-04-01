#pragma once

#include "Object.h"
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
std::vector<std::string> split(const std::string& source, const std::string& s);

template<typename... Args>
struct VisaAdl<std::tuple<Args...>>
{
    inline static std::string toScpi(const std::tuple<Args...>& arg)
    {
        static_assert(std::_Always_false<std::tuple<Args...>>, "Please define your ADL function: std::string toScpi(YourType).");
    }
    inline static void fromScpi(const std::string& ret, std::tuple<Args...>& out)
    {
        out = _TupleHelper<std::tuple<Args...>>(split(ret, ","));
    }
};

template<>
struct VisaAdl<bool>
{
    static void fromScpi(const std::string& ret, bool& val);
    static std::string toScpi(bool source);
};

template<>
struct VisaAdl<double>
{
    static void fromScpi(const std::string& ret, double& val);
    static std::string toScpi(double source);
};

template<>
struct VisaAdl<int>
{
    static void fromScpi(const std::string& ret, int& val);
    static std::string toScpi(int source);
};

template<typename T>
inline std::string encode(const T& arg)
{
    return VisaAdl<T>::toScpi(arg);
}

template<typename T>
inline T decode(const std::string& ret)
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

//跳过元组的前i项，生成一个新的元组,如{1,2,3}调用skip<1>时，返回{2,3}.
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
    struct SubClass : public Visa::SampleValue<ValueType>                                                                                  \
    {                                                                                                                                      \
        using SampleValue<ValueType>::operator ValueType;                                                                                  \
        using SampleValue<ValueType>::operator=;                                                                                           \
    }
