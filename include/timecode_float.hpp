// Copyright (C) Stefan Olivier
// <https://stefanolivier.com>
// ----------------------------
// Description: Timecode library templates for floating point implementation

#pragma once

///////////////////////////////////////////////////////////////////////////

// Standard headers
#include <algorithm>
#include <array>
#include <cmath>
#include <compare>
#include <concepts>
#include <limits>
#include <ranges>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

// Project headers
#include "errors.hpp"
#include "timecode_common.hpp"
#include "traits.hpp"
#include "utility.hpp"

///////////////////////////////////////////////////////////////////////////

#ifndef VTM_TIMECODE_FLOAT_MACROS
#define VTM_TIMECODE_FLOAT_MACROS


#ifndef VTM_TIMECODE_FLOAT_PRECISION_MARGIN
#define VTM_TIMECODE_FLOAT_PRECISION_MARGIN 0.0001
#endif

#ifndef VTM_TIMECODE_FLOAT_ROUNDING_PRECISION
#define VTM_TIMECODE_FLOAT_ROUNDING_PRECISION 100000000
#endif

#endif // @END OF VTM_TIMECODE_FLOAT_MACROS

///////////////////////////////////////////////////////////////////////////


namespace vtm::chrono {

// @SECTION: Timecode Functions
template<vtm::traits::StringLike S>
inline auto is_tcstring_dropframe(const S& tc) -> bool
{
    VTM_ASSERT(tc.length() >= 11, "length of timecode string is not greater than or equal to 11");
    const int msf_length = 9; // length (:mm:ss:ff) = 9
    const int hours_length = tc.length() - msf_length;
    
    if (tc[hours_length + 6] == ';') return true;

    return false;
}

template<vtm::traits::StringLike S = std::string>
inline auto get_tc_delimiter(const vtm::traits::StringLike auto& tc) -> S
{
    return (is_tcstring_dropframe(tc)) ? ";" : ":";
}

template<vtm::traits::StringLike S = std::string>
inline auto get_tc_delimiter(bool is_dropframe) -> S
{
    return (is_dropframe) ? ";" : ":";
}

template<std::floating_point F>
inline F tc_round(const F f)
{
    return std::round(f * VTM_TIMECODE_FLOAT_ROUNDING_PRECISION) / VTM_TIMECODE_FLOAT_ROUNDING_PRECISION;
}

template<std::floating_point F>
inline F fps_to_single_tick(const F fps)
{
    VTM_ASSERT(fps > 0.0, "Frame rate must be a non-zero floating point value");
    return 1.0 / fps / 100.0;
}

template<std::floating_point T, std::same_as<T>... Args>
inline auto chunks_to_total_ticks(const std::tuple<Args...>& time, const T fps)
{
    VTM_ASSERT(fps > 0.0, "Frame rate must be a non-zero floating point value");
    const auto& [ h, m, s, f ] = time;
    T total_frames = (((h * 60.0 * 60.0) + (m * 60.0) + s) * fps) + f;
    T ticks = fps_to_single_tick(fps) * total_frames;
    return ticks;
}

template<std::floating_point T>
inline auto fps_to_ticks_by_chunk(const T fps) -> std::array<T, 4>
{
    VTM_ASSERT(fps > 0.0, "frame rate must be a non-zero floating point value");
    using float_t = T;
    const float_t ht = tc_round(fps_to_single_tick(fps) * 60.0 * 60.0 * fps);
    const float_t mt = tc_round(fps_to_single_tick(fps) * 60.0 * fps);
    const float_t st = tc_round(fps_to_single_tick(fps) * fps);
    const float_t ft = tc_round(fps_to_single_tick(fps));

    return std::array{ht, mt, st, ft};
}

template<vtm::traits::StringLike S, std::floating_point F>
inline auto ticks_to_chunk_string(const F n, const F k, bool round = false) -> S
{
    using float_t = F;
    using string_t = S;
    string_t str = "00";
    float_t d = (round) ? std::round(n / k) : n / k;

    if (d > n / k) return str;
    str = std::to_string(d);
    str = str.substr(0, str.find_first_of('.', 0));

    return (str.length() == 1) ? ("0" + str) : str;
}

template<vtm::traits::StringLike S, std::floating_point F>
auto ticks_to_string(F n, const F fps, bool is_dropframe = false) -> S
{
    VTM_ASSERT(fps > 0.0, "frame rate must be a non-zero floating point value");

    using string_t = S;
    using float_t = F;

    const auto& [ht, mt, st, ft] = fps_to_ticks_by_chunk(fps);
    string_t h = ticks_to_chunk_string<string_t>(n, ht); n = std::fmod(n, ht);
    string_t m = ticks_to_chunk_string<string_t>(n, mt); n = std::fmod(n, mt);
    string_t s = ticks_to_chunk_string<string_t>(n, st); n = std::fmod(n, st);
    string_t f = ticks_to_chunk_string<string_t>(n, ft, true);

    return h + ":" + m + ":" + s + get_tc_delimiter(is_dropframe) + f;
}

template<vtm::traits::StringLike S>
inline auto valid_tcstring(const S& tc) -> bool
{
    namespace rg = std::ranges;
    bool valid = !rg::empty(tc);
    if (!valid) return false;

    std::size_t chunk_count = 0;
    std::array<std::size_t, 4> chunk_sizes{ 0, 2, 2, 2 };
    const auto delim = get_tc_delimiter(tc);

    for (const auto& ch : rg::split_view(tc, delim)) {
        const std::string_view sv(ch.begin(), ch.end());

        if (chunk_sizes[chunk_count] > 0)
            valid = valid && sv.length() == chunk_sizes[chunk_count];

        for (const auto& d : sv) {
            valid = valid && std::isdigit(d);
            if (!valid) return false;
        }

        ++chunk_count;
    }

    return valid && chunk_count == 4;
}

template<vtm::traits::StringLike S, std::floating_point F>
inline auto tcstring_to_ticks(const S& tc, const F fps) -> F
{
    namespace rg = std::ranges;
    using string_t = S;
    using float_t = F;

    VTM_ASSERT(valid_tcstring(tc) == true, "invalid timecode string was parsed");

    const auto delim = get_tc_delimiter(tc);
    const auto coefs = fps_to_ticks_by_chunk(fps);
    float_t ticks = 0.0;

    std::size_t i = 0;
    for (const auto& ch : rg::split_view(tc, delim)) {
        const std::string_view chunk(ch.begin(), ch.end());
        ticks += vtm::string_to_float<float_t>(chunk) * coefs[i++];
    }
    
    return ticks;
}

} // @END OF namespace vtm::chrono

///////////////////////////////////////////////////////////////////////////

namespace vtm::chrono::internal {

template<typename T>
concept BasicTimecodeStringAliases = requires (T t) {
    vtm::traits::StringLike<typename T::string_t>;
    vtm::traits::StringLike<typename T::string_view_t>;
};

template<typename T>
concept BasicTimecodeCompatible = BasicTimecodeStringAliases<T>
                               && vtm::traits::InterfaceReset<T>
                               && vtm::traits::InterfaceDisplay<T>
                               && requires {
                                  typename T::display_t;
                               };

template<typename T1, typename T2>
consteval auto swap_types(T1 f1, T2 f2) -> std::tuple<T2, T1>
{
    return std::make_tuple(f2, f1);
}

template<typename... Ts, std::invocable... Fs>
consteval auto match_type_pattern(std::tuple<Ts...> ts, std::tuple<Fs...> fs)
{
    static_assert(std::tuple_size_v<decltype(fs)> >= std::tuple_size_v<decltype(ts)>,
                  "total types for Fs must be greater than or equal to total types for Ts");
    
    using ts_type = decltype(ts);
    using fs_type = decltype(fs);

    constexpr std::size_t ts_size = std::tuple_size_v<ts_type>;
    constexpr std::size_t fs_size = std::tuple_size_v<fs_type>;

    // TODO: SAVE
}

template<typename L,  typename R>
struct paired_members
{
    template<std::invocable F1, std::invocable F2, std::invocable... Fs>
    static constexpr std::invocable
    auto choose(F1&& f1, F2&& f2, Fs&&... fs)
    {
        if constexpr(std::is_same_v<std::invoke_result_t<F1>, L>
                  && std::is_same_v<std::invoke_result_t<F2>, R>)
        {
            return [=]() { return std::make_pair(f1(), f2()); };
        }

        else if constexpr(std::is_same_v<std::invoke_result_t<F1>, R>
                       && std::is_same_v<std::invoke_result_t<F2>, L>)
        {
            return [=]() { return std::make_pair(f2(), f1()); };
        }

        else if constexpr((std::is_same_v<std::invoke_result_t<F1>, L>
                      &&  !std::is_same_v<std::invoke_result_t<F2>, R>)
                      ||  (std::is_same_v<std::invoke_result_t<F1>, R>
                      &&  !std::is_same_v<std::invoke_result_t<F2>, L>))
        {
            return paired_members<L, R>::choose(std::forward<F1>(f1),
                                                std::forward<Fs>(fs)...);
        }

        else if constexpr( (!std::is_same_v<std::invoke_result_t<F1>, L>
                        &&   std::is_same_v<std::invoke_result_t<F2>, R>)
                        || (!std::is_same_v<std::invoke_result_t<F1>, R>
                        &&   std::is_same_v<std::invoke_result_t<F2>, L>))
        {
            return paired_members<L, R>::choose(std::forward<F2>(f2),
                                                std::forward<Fs>(fs)...);
        }

        else {
            return paired_members<L, R>::choose(std::forward<Fs>(fs)...);
        }
    }
};

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION __BasicTimecodeFloat -- 
//
///////////////////////////////////////////////////////////////////////////

template<vtm::traits::StringLike TString,
         vtm::traits::StringLike TView,
         std::signed_integral TInt,
         std::floating_point TFloat,
         FpsFormatFactory TFps>
class __BasicTimecodeFloat : public vtm::traits::__reset
                           , public vtm::traits::__implicit_string_overloads<TString, TView>
                           , public vtm::traits::__display<typename vtm::traits::__implicit_string_overloads<TString, TView>::string_view_type>
                           , public vtm::traits::__convert_to_float<__BasicTimecodeFloat<TString, TView, TInt, TFloat, TFps>, TFloat>
                           , public vtm::traits::__convert_to_signed<__BasicTimecodeFloat<TString, TView, TInt, TFloat, TFps>, TInt>
                           , public vtm::traits::__convert_to_string<__BasicTimecodeFloat<TString, TView, TInt, TFloat, TFps>, TString>
{

///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Type Aliases -- 
//
///////////////////////////////////////////////////////////////////////////

public:
    using __my_type     = __BasicTimecodeFloat<TString, TView, TInt, TFloat, TFps>;
    using string_t      = typename vtm::traits::__implicit_string_overloads<TString, TView>::string_type;
    using string_view_t = typename vtm::traits::__implicit_string_overloads<TString, TView>::string_view_type;
    using display_t     = string_view_t;
    using signed_type   = typename vtm::traits::__convert_to_signed<__my_type, TInt>::signed_type;
    using float_type    = typename vtm::traits::__convert_to_float<__my_type, TFloat>::float_type;
    using string_type   = typename vtm::traits::__convert_to_string<__my_type, string_t>::string_type;
    using fps_factory_t = TFps;
    using fps_t         = typename TFps::type;
    using __element1_type = float_t;
    using __element2_type = fps_t;

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Static Methods -- 
//
///////////////////////////////////////////////////////////////////////////

public:
    // Static members
    template<std::integral V>
    static auto from_hmsf(const V h,
                          const V m,
                          const V s,
                          const V f,
                          const fps_t fps = fps_factory_t::default_value()) -> __BasicTimecodeFloat
    {
        VTM_ASSERT(h >= 0, "timecode hours must be greater than or equal to zero");
        VTM_ASSERT(m >= 0, "timecode minutes must be greater than or equal zero");
        VTM_ASSERT(s >= 0, "timecode seconds must be greater than or equal zero");
        VTM_ASSERT(f >= 0, "timecode frames must be greater than or equal zero");

        return __BasicTimecodeFloat {
            chunks_to_total_ticks(std::make_tuple(float_type(h), float_type(m), float_type(s), float_type(f)),
                                  __FPSFORMAT_VALUE_TO_FLOAT(fps)),
            fps
        };
    }

    template<vtm::traits::StringConstructible S>
    static auto from_string(const S& tc, const fps_t fps = fps_factory_t::default_value()) -> __BasicTimecodeFloat
    {
        const char* msg = "cannot create new timecode object with invalid timecode string";

        if constexpr (std::same_as<string_t, S> || std::same_as<string_view_t, S>){
            VTM_ASSERT(valid_tcstring(tc), msg);
            return __BasicTimecodeFloat { tcstring_to_ticks(tc, __FPSFORMAT_VALUE_TO_FLOAT(fps)),
                                          fps };
        }

        else {
            VTM_ASSERT(valid_tcstring(string_t(tc)), msg);
            return __BasicTimecodeFloat { tcstring_to_ticks(string_t(tc), __FPSFORMAT_VALUE_TO_FLOAT(fps)), 
                                          fps };
        }
    }

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Virtual Methods --
//
///////////////////////////////////////////////////////////////////////////

public:
    void reset() noexcept
    {
        this->set_value(0.0);
    }

    auto display() const -> display_t { VTM_TODO("not implemented"); }

    // TODO: Test that this doesn't stack overflow because of implicit type conversion
    explicit operator string_view_t() const { return string_type(*this); }

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Ctors, Dtors & Assignment --
//
///////////////////////////////////////////////////////////////////////////

public:
    __BasicTimecodeFloat() = default;
    ~__BasicTimecodeFloat() = default;

    __BasicTimecodeFloat(const __BasicTimecodeFloat& tc)
        : _value(tc._value)
        , _fps(tc._fps)
    {}

    __BasicTimecodeFloat(__BasicTimecodeFloat&& tc) noexcept
    {
        this->_value = tc._value;
        this->_fps = tc._fps;
        tc._value = {};
        tc._fps = {};
    }

    __BasicTimecodeFloat& operator=(const __BasicTimecodeFloat& tc)
    {
        this->_value = tc._value;
        this->_fps = tc._fps;
        return *this;
    }

    __BasicTimecodeFloat& operator=(__BasicTimecodeFloat&& tc) noexcept
    {
        this->_value = tc._value;
        this->_fps = tc._fps;
        tc._value = {};
        tc._fps = {};
        return *this;
    }

    template<TimecodePrimitive V>
    explicit __BasicTimecodeFloat(const V value, const fps_t fps = fps_factory_t::default_value()) noexcept
        : _value(value)
        , _fps(fps)
    {}

    explicit constexpr __BasicTimecodeFloat(const string_view_t& tc)
    {
        VTM_TODO("not implemented");
    }

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Accessors & Mutators --
//
///////////////////////////////////////////////////////////////////////////

public:
    // Accessor methods
    // TODO: Frame rate aware set_value()
    void set_value(const __BasicTimecodeFloat& value) noexcept
    {
        this->_value = value._value;
    }

    template<TimecodePrimitive V>
    void set_value(const V value) noexcept
    {
        this->_value = value;
    }

    void set_fps(const fps_t fps) noexcept
    {
        this->_fps = fps;
    }

    auto fps() const noexcept -> fps_t
    {
        return this->_fps;
    }

    auto is_drop_frame() const noexcept -> bool
    {
        return fps_factory_t::is_drop_frame(this->_fps);
    }

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
// -- @SECTION Implicit Type Conversions --
//
///////////////////////////////////////////////////////////////////////////

public:
    operator signed_type() const
    {
        return static_cast<signed_type>(std::round(this->_value));
    }

    operator float_type() const
    {
        return this->_value;
    }

    operator string_type() const
    {
        return ticks_to_string<string_t>(this->as_float(),
                                         fps_factory_t::to_float(this->_fps),
                                         fps_factory_t::is_drop_frame(this->_fps));
    }

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Conversion Methods --
//
///////////////////////////////////////////////////////////////////////////

public:
    template<typename L = float_type, typename R = string_t>
    auto as_pair() const -> std::pair<L, R>
    {
        return paired_members<L, R>::choose([this]() { return this->as_signed(); },
                                            [this]() { return this->as_float();  },
                                            [this]() { return this->as_string(); },
                                            [this]() { return this->fps();       })();
    }

    template<typename A = float_type, typename B = fps_t, typename C = string_t>
    auto as_tuple() const -> std::tuple<A, B, C>
    {
        VTM_TODO("not implemented");
    }

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Arithmetic & Assigment Operations --
//
///////////////////////////////////////////////////////////////////////////

public:
    // TODO: Frame rate aware assignments and arithmetic
    template<TimecodePrimitive V>
    __BasicTimecodeFloat& operator=(const V rhs)
    {
        this->set_value(rhs);
        return *this;
    }

    __BasicTimecodeFloat& operator+=(const auto& rhs)
    {
        this->set_value(*this + rhs);
        return *this;
    }

    __BasicTimecodeFloat& operator-=(const auto& rhs)
    {
        this->set_value(*this - rhs);
        return *this;
    }

    template<TimecodePrimitive V>
    friend __BasicTimecodeFloat operator+(const __BasicTimecodeFloat& lhs, const V rhs)
    {
        __BasicTimecodeFloat tmp{ lhs };

        tmp.set_value(std::min(std::numeric_limits<float_type>::max(),
                      tmp._value + rhs));

        return tmp;
    }

    template<TimecodePrimitive V>
    friend V operator+(const V lhs, const __BasicTimecodeFloat& rhs)
    {
        return lhs + rhs._value;
    }

    template<TimecodePrimitive V>
    friend __BasicTimecodeFloat operator-(const __BasicTimecodeFloat& lhs, const V rhs)
    {
        __BasicTimecodeFloat tmp{ lhs };

        tmp.set_value(std::max(std::numeric_limits<float_type>::min(),
                      tmp._value - rhs));

        return tmp;
    }

    template<TimecodePrimitive V>
    friend V operator-(const V lhs, const __BasicTimecodeFloat& rhs)
    {

        return lhs - rhs._value;
    }

    friend __BasicTimecodeFloat operator+(const __BasicTimecodeFloat& lhs, const __BasicTimecodeFloat& rhs)
    {
        __BasicTimecodeFloat tmp { lhs };

        tmp.set_value(std::min(std::numeric_limits<float_type>::max(),
                      tmp._value + rhs._value));

        return tmp;
    }

    friend __BasicTimecodeFloat operator-(const __BasicTimecodeFloat& lhs, const __BasicTimecodeFloat& rhs)
    {
        __BasicTimecodeFloat tmp { lhs };

        tmp.set_value(std::max(std::numeric_limits<float_type>::min(),
                      tmp._value - rhs._value));

        return tmp;
    }

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Equality Comparison --
//
///////////////////////////////////////////////////////////////////////////

public:
    bool operator==(const __BasicTimecodeFloat& rhs) const
    {
        return this->_value == rhs._value && this->_fps == rhs._fps;
    }

    auto operator<=>(const __BasicTimecodeFloat& rhs) const
    {
        // TODO: Integrate fps to partial ordering
        std::partial_ordering ordering = this->_value <=> rhs._value;
        bool fps_eq = this->_fps == rhs._fps;
        if (ordering < 0 && fps_eq) return std::partial_ordering::less;
        if (ordering > 0 && fps_eq) return std::partial_ordering::greater;
        if (ordering == 0 && fps_eq) return std::partial_ordering::equivalent;
        return std::partial_ordering::unordered;
    }

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Standard Library get() Overloads --
//
///////////////////////////////////////////////////////////////////////////

public:
    template<std::size_t Index>
    auto&& get()        & { return this->get_helper<Index>(*this); }

    template<std::size_t Index>
    auto&& get()       && { return this->get_helper<Index>(*this); }

    template<std::size_t Index>
    auto&& get()  const & { return this->get_helper<Index>(*this); }

    template<std::size_t Index>
    auto&& get() const && { return this->get_helper<Index>(*this); }


private:
    template<std::size_t Index, typename T>
    auto&& get_helper(T&& t) const
    {
        static_assert(Index < 2, "index out of bounds for __BasicTimecodeFloat");
        if constexpr (Index == 0) return std::forward<T>(t)._value;
        if constexpr (Index == 1) return std::forward<T>(t)._fps;
    }

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Private Data Members --
//
///////////////////////////////////////////////////////////////////////////

private:
    // @SECTION __BasicTimecodeFloat Data Members
    float_type _value = 0.0;
    fps_t _fps = fps_factory_t::default_value();

};

///////////////////////////////////////////////////////////////////////////

} // @END OF namespace vtm::chrono::internal
