#pragma once

#include <tuple>

template <typename... Args>
using TypeTuple = std::tuple<Args...>;

template <typename T, typename TT>
inline T& get(const TT &tt)
{
    constexpr size_t i =   std::tuple_size<TT>();
    return std::get<i>(tt);
}