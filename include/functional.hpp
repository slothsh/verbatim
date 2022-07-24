// Copyright (C) Stefan Olivier
// <https://stefanolivier.com>
// ----------------------------
// Description: Generic functional-style functions

#pragma once

///////////////////////////////////////////////////////////////////////////

// Standard headers
#include <cstdint>
#include <concepts>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

// Project headers
#include "utility.hpp"

///////////////////////////////////////////////////////////////////////////

namespace vtm::functional {

///////////////////////////////////////////////////////////////////////////
//
//                    @SECTION Chain
//
///////////////////////////////////////////////////////////////////////////

template<std::invocable D>
constexpr auto chain(D d)
{
    return d();
}

template<std::invocable D,
         std::invocable<std::invoke_result_t<D>> F,
         std::invocable<std::invoke_result_t<D>>... Fs>
constexpr auto chain(D d, F f, Fs... fs)
{
    return chain([=](){ return f(d()); }, fs...);
}

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//                    @SECTION Swap
//
///////////////////////////////////////////////////////////////////////////

namespace internal {

template<typename... Ts, std::size_t... Ns>
constexpr auto swap_impl(std::tuple<Ts...> ts,
                         std::index_sequence<Ns...> seq)
{
    static_assert(sizeof...(Ts) >= 2, "size of tuple must be 2 or greater");
    return std::tuple{ std::get<Ns>(ts)... };
}

} // @END OF namespace vtm::functional::internal

template<std::size_t I = 0, typename... Ts, std::size_t Size = sizeof...(Ts)>
constexpr auto swap(std::tuple<Ts...> ts)
{
    static_assert(sizeof...(Ts) >= 2, "size of tuple must be 2 or greater");
    static_assert(I < sizeof...(Ts), "swap index must be less than total size of tuple");

    constexpr auto mapping = [](std::size_t seq_i) {
        if (seq_i == 0 && I == Size - 1) {
            return Size - 1;
        }

        if (seq_i == I) {
            if (seq_i == Size - 1) return std::size_t{0};
            return seq_i + 1;
        }

        if (seq_i == I + 1) {
            if (seq_i == Size) return Size - 1;
            return seq_i - 1;
        }

        return seq_i;
    };

    constexpr auto seq = vtm::utility::map_integer_sequence(mapping,
                                                            std::make_integer_sequence<std::size_t, Size>{});
    return internal::swap_impl(std::forward<decltype(ts)>(ts), seq);
}

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//                    @SECTION Head & Tails
//
///////////////////////////////////////////////////////////////////////////

template<typename... Ts>
constexpr auto head(std::tuple<Ts...> ts)
{
    constexpr std::size_t size_ts = sizeof...(Ts);
    static_assert(size_ts > 0, "size of tuple must be greater than 1");
    return std::get<0>(ts);
}

template<typename... Ts>
constexpr auto tail(std::tuple<Ts...> ts)
{
    constexpr std::size_t size_ts = sizeof...(Ts);
    static_assert(size_ts > 0, "size of tuple must be greater than 1");
    return std::get<size_ts - 1>(ts);
}

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//                    @SECTION Headless & Tailless
//
///////////////////////////////////////////////////////////////////////////

namespace internal {

template<typename... Ts, std::size_t... Ns>
constexpr auto headless_impl(std::tuple<Ts...> ts,
                        std::index_sequence<Ns...> seq)
{
    static_assert(sizeof...(Ts) > 0, "size of tuple must be greater than 1");
    return std::tuple{ std::get<(Ns + 1 < sizeof...(Ts)) ? Ns + 1 : Ns>(ts)... };
}

template<typename... Ts, std::size_t... Ns>
constexpr auto tailless_impl(std::tuple<Ts...> ts,
                        std::index_sequence<Ns...> seq)
{
    static_assert(sizeof...(Ts) > 0, "size of tuple must be greater than 1");
    return std::tuple{ std::get<(Ns - 1 < 0) ? 0 : Ns>(ts)... };
}

} // @END OF namespace vtm::functional::internal

template<typename... Ts>
constexpr auto headless(std::tuple<Ts...> ts)
{
    static_assert(sizeof...(Ts) > 0, "size of tuple must be greater than 1");
    constexpr auto seq = std::make_index_sequence<sizeof...(Ts)>{};
    return internal::headless_impl(std::forward<decltype(ts)>(ts), seq);
}

template<typename... Ts>
constexpr auto tailless(std::tuple<Ts...> ts)
{
    static_assert(sizeof...(Ts) > 0, "size of tuple must be greater than 1");
    constexpr auto seq = std::make_index_sequence<sizeof...(Ts)>{};
    return internal::tailless_impl(std::forward<decltype(ts)>(ts), seq);
}

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//                    @SECTION Append
//
///////////////////////////////////////////////////////////////////////////

namespace internal {

template<typename... Ts, typename T, std::size_t... Ns>
constexpr auto append_impl(std::tuple<Ts...> ts,
                      T t,
                      std::index_sequence<Ns...> seq)
{
    static_assert(sizeof...(Ts) > 0, "size of tuple must be greater than 1");
    return std::tuple{ std::get<Ns>(ts)..., t };
}

} // @END OF namespace vtm::functional::internal

template<typename... Ts, typename T>
constexpr auto append(std::tuple<Ts...> ts, T t)
{
    static_assert(sizeof...(Ts) > 0, "size of tuple must be greater than 1");
    constexpr auto seq = std::make_index_sequence<sizeof...(Ts)>{};
    return internal::append_impl(std::forward<decltype(ts)>(ts), std::forward<T>(t), seq);
}

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//                    @SECTION Prepend
//
///////////////////////////////////////////////////////////////////////////

namespace internal {

template<typename... Ts, typename T, std::size_t... Ns>
constexpr auto prepend_impl(std::tuple<Ts...> ts,
                      T t,
                      std::index_sequence<Ns...> seq)
{
    static_assert(sizeof...(Ts) > 0, "size of tuple must be greater than 1");
    return std::tuple{ t, std::get<Ns>(ts)...};
}

} // @END OF namespace vtm::functional::internal

template<typename... Ts, typename T>
constexpr auto prepend(std::tuple<Ts...> ts, T t)
{
    constexpr auto seq = std::make_index_sequence<sizeof...(Ts) - 1>{};
    return internal::prepend_impl(std::forward<decltype(ts)>(ts), std::forward<T>(t), seq);
}

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//                    @SECTION Rotate Left & Right
//
///////////////////////////////////////////////////////////////////////////

template<typename... Ts>
constexpr auto rotl(std::tuple<Ts...> ts)
{
    static_assert(sizeof...(Ts) > 0, "size of tuple must be greater than 1");
    const auto h = head(ts);
    const auto rest = headless(ts);
    return append(rest, h);
}

template<typename... Ts>
constexpr auto rotr(std::tuple<Ts...> ts)
{
    static_assert(sizeof...(Ts) > 0, "size of tuple must be greater than 1");
    const auto t = tail(ts);
    const auto rest = tailless(ts);
    return prepend(rest, t);
}

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//                    @SECTION Range
//
///////////////////////////////////////////////////////////////////////////

namespace internal {

constexpr auto range_impl(std::size_t start, std::size_t end) {
    constexpr auto max_size = std::numeric_limits<std::size_t>::max();

    start = std::min<std::size_t>(
        std::max(std::size_t{0}, start),
        max_size
    );

    end = std::min(end, max_size);
    const std::size_t range = end - start;
    return std::views::iota(start, end) | std::views::take(range);

}

} // @END OF namespace vtm::functional::internal

constexpr auto range(std::size_t end)
{
    return internal::range_impl(0, end);
}

constexpr auto range(std::size_t start, std::size_t end)
{
    return internal::range_impl(start, end);
}

template<template<typename, std::size_t> typename C, typename T, std::size_t N>
    requires std::ranges::sized_range<C<T, N>>
constexpr static auto enumerate(const C<T, N>& in)
{
    std::size_t i = 0;
    std::array<std::pair<std::size_t, T>, N> out;

    for (const auto& e : in) {
        out[i] = std::make_pair(i, e);
        ++i;
    }

    return out;
}

template<template<typename> typename C, typename T>
    requires std::ranges::sized_range<C<T>>
constexpr static auto enumerate(const C<T>& in)
{
    std::size_t i = 0;
    std::vector<std::pair<std::size_t, T>> out{};

    for (const auto& e : in) {
        out.emplace_back(std::make_pair(i++, e));
    }

    return out;
}

///////////////////////////////////////////////////////////////////////////

} // @END OF namspace vtm::functional

