#pragma once

#include <tuple>

namespace red
{
template <typename... T>
using Tuple = std::tuple<T...>;

template <typename... T>
using TupleSize = std::tuple_size<T...>;

template <typename T1, typename T2>
using Pair = std::pair<T1, T2>;

template <bool...>
struct bool_pack;
template <bool... bs>
using all_true = std::is_same<bool_pack<bs..., true>, bool_pack<true, bs...>>;

template <std::size_t I = 0, typename FuncT, typename... Tp>
inline constexpr typename std::enable_if<I == sizeof...(Tp), void>::type for_each(
    std::tuple<Tp...>&, FuncT)  // Unused arguments are given no names.
{
}

template <std::size_t I = 0, typename FuncT, typename... Tp>
    inline constexpr typename std::enable_if < I<sizeof...(Tp), void>::type for_each(std::tuple<Tp...>& t, FuncT f)
{
    f(I, std::get<I>(t));
    for_each<I + 1, FuncT, Tp...>(t, f);
}

template <std::size_t I = 0, typename T1, typename T2>
inline constexpr typename std::enable_if<I == TupleSize<T1>::value, void>::type cast_tuple(
    T1&, T2&)  // Unused arguments are given no names.
{
}

template <std::size_t I = 0, typename T1, typename T2>
    inline constexpr typename std::enable_if < I<TupleSize<T1>::value, void>::type cast_tuple(T1& input, T2& output)
{
    std::get<I>(output) = reinterpret_cast<std::remove_reference_t<decltype(std::get<I>(output))>>(std::get<I>(input));
    cast_tuple<I + 1, T1, T2>(input, output);
}

}  // namespace red