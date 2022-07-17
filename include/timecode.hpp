// Copyright (C) Stefan Olivier
// <https://stefanolivier.com>
// ----------------------------
// Description: Timecode library object

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
#include "traits.hpp"
#include "utility.hpp"

///////////////////////////////////////////////////////////////////////////

#ifndef VTM_TIMECODE_MACROS
#define VTM_TIMECODE_MACROS

#ifndef VTM_TIMECODE_PRECISION_MARGIN
#define VTM_TIMECODE_PRECISION_MARGIN 0.0001
#endif

#ifndef VTM_TIMECODE_ROUNDING_PRECISION
#define VTM_TIMECODE_ROUNDING_PRECISION 100000000
#endif

#ifndef VTM_TIMECODE_FPS_DEFAULT
#define VTM_TIMECODE_FPS_DEFAULT 25
#endif

#define __FPS_TYPE() vtm::chrono::internal::__FPSFormat<vtm::chrono::float64_t, vtm::chrono::int64_t>::type

#define __FPSFORMAT_VALUE_TO_STRING(in) GET_ENUM_MAPPING_1(__FPSFORMAT_STRING(), in)
#define __FPSFORMAT_STRING_TO_VALUE(in) GET_ENUM_MAPPING_2(__FPSFORMAT_STRING(), in)
#define __FPSFORMAT_STRING() DECLARE_ENUM_MAPPING(__FPS_TYPE(),                            \
                                                  std::string_view,                        \
                                                  [](){ VTM_WARN("unknown fps format"); }, \
                                                  [](){ VTM_WARN("unknown fps format"); }, \
                                                  __FPS_TYPE()::none,                      \
                                                  "NONE",                                  \
                                                  "24 fps",                                \
                                                  "25 fps",                                \
                                                  "30 fps",                                \
                                                  "29.97 fps",                             \
                                                  "29.97 fps drop-frame",                  \
                                                  "60 fps")

#define __FPSFORMAT_VALUE_TO_INT(in) GET_ENUM_MAPPING_1(__FPSFORMAT_INT(), in)
#define __FPSFORMAT_INT_TO_VALUE(in) GET_ENUM_MAPPING_2(__FPSFORMAT_INT(), in)
#define __FPSFORMAT_INT() DECLARE_ENUM_MAPPING(__FPS_TYPE(),                            \
                                               vtm::chrono::int64_t,                    \
                                               [](){ VTM_WARN("unknown fps format"); }, \
                                               [](){ VTM_WARN("unknown fps format"); }, \
                                               __FPS_TYPE()::none,                      \
                                               0,                                       \
                                               24,                                      \
                                               25,                                      \
                                               30,                                      \
                                               29,                                      \
                                               -29,                                     \
                                               60)

#define __FPSFORMAT_VALUE_TO_FLOAT(in) GET_ENUM_MAPPING_1(__FPSFORMAT_FLOAT(), in)
#define __FPSFORMAT_FLOAT_TO_VALUE(in) GET_ENUM_MAPPING_2(__FPSFORMAT_FLOAT(), in)
#define __FPSFORMAT_FLOAT() DECLARE_ENUM_MAPPING(__FPS_TYPE(),                            \
                                                 vtm::chrono::float64_t,                  \
                                                 [](){ VTM_WARN("unknown fps format"); }, \
                                                 [](){ VTM_WARN("unknown fps format"); }, \
                                                 __FPS_TYPE()::none,                      \
                                                 0.0,                                     \
                                                 24.0,                                    \
                                                 25.0,                                    \
                                                 30.0,                                    \
                                                 29.97,                                   \
                                                 -29.97,                                  \
                                                 60.0)

#define __FPSFORMAT_VALUE_TO_DROPFRAME(in) GET_ENUM_MAPPING_1(__FPSFORMAT_DROPFRAME(), in)
#define __FPSFORMAT_DROPFRAME_TO_VALUE(in) GET_ENUM_MAPPING_2(__FPSFORMAT_DROPFRAME(), in)
#define __FPSFORMAT_DROPFRAME() DECLARE_ENUM_MAPPING(__FPS_TYPE(),                         \
                                                  bool,                                    \
                                                  [](){ VTM_WARN("unknown fps format"); }, \
                                                  [](){ VTM_WARN("unknown fps format"); }, \
                                                  __FPS_TYPE()::none,                      \
                                                  false,                                   \
                                                  false,                                   \
                                                  false,                                   \
                                                  false,                                   \
                                                  false,                                   \
                                                  true,                                    \
                                                  false)

#endif // @END OF VTM_TIMECODE_MACROS

///////////////////////////////////////////////////////////////////////////

namespace vtm::chrono {

using float64_t = long double;
using int64_t = long long;

} // @END OF namespace chrono

///////////////////////////////////////////////////////////////////////////

namespace vtm::chrono::internal {

// @SECTION: Default FPS Format Template
template<typename T>
concept FpsFormatFactory = std::is_enum_v<typename T::format>
                        && std::is_same_v<typename T::type, typename T::format>
                        && requires (T t, typename T::type type) {
                               std::floating_point<typename T::float_type>;
                               std::integral<typename T::int_type>;
                               { T::from_int(0) } -> std::same_as<typename T::type>;
                               { T::from_float(0.0) } -> std::same_as<typename T::type>;
                               { T::from_string(std::string_view("")) } -> std::same_as<typename T::type>;
                               { T::from_string(std::string("")) } -> std::same_as<typename T::type>;
                               { T::to_int(type) } -> std::integral;
                               { T::to_float(type) } -> std::floating_point;
                               { T::to_string(type) } -> vtm::traits::StringLike;
                           };

template<std::floating_point TFloat, std::integral TInt> // TODO: Constraint for Derived class
struct __FPSFormat
{
    enum format : TInt {
        fps_24,
        fps_25,
        fps_30,
        fps_29p97,
        fpsdf_29p97,
        fps_60,
        none
    };
    
    using __my_type = __FPSFormat<TFloat, TInt>;
    using type = format;
    using float_type = TFloat;
    using int_type = TInt;

    static constexpr auto default_value() -> type
    {
        return __FPSFORMAT_INT_TO_VALUE(VTM_TIMECODE_FPS_DEFAULT);
    }

    static constexpr auto is_drop_frame(const type& t) -> bool
    {
        return __FPSFORMAT_VALUE_TO_DROPFRAME(t);
    }

    static constexpr auto from_int(std::integral auto const i) -> type
    {
        return __FPSFORMAT_INT_TO_VALUE(i);
    }

    static constexpr auto from_float(std::floating_point auto const f) -> type
    {
        return __FPSFORMAT_FLOAT_TO_VALUE(f);
    }

    static constexpr auto from_string(vtm::traits::StringLike auto const& s) -> type
    {
        return __FPSFORMAT_STRING_TO_VALUE(s);
    }

    static constexpr std::integral
    auto to_int(const type& t)
    {
        return std::abs(__FPSFORMAT_VALUE_TO_INT(t));
    }

    static constexpr std::floating_point
    auto to_float(const type& t)
    {
        return std::abs(__FPSFORMAT_VALUE_TO_FLOAT(t));
    }

    static constexpr vtm::traits::StringLike
    auto to_string(const type& t)
    {
        return __FPSFORMAT_VALUE_TO_STRING(t);
    }
};

} // @END OF namespace vtm::chrono::internal

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
    return std::round(f * VTM_TIMECODE_ROUNDING_PRECISION) / VTM_TIMECODE_ROUNDING_PRECISION;
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

// @SECTION: Basic Timecode Object

template<typename T>
concept TimecodePrimitive = std::floating_point<T> || std::integral<T>;

template<vtm::traits::StringLike TString>
inline constexpr auto basic_timecode_default_display() -> TString
{
    return "00:00:00:00";
}

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
//           -- @SECTION __BasicTimecode -- 
//
///////////////////////////////////////////////////////////////////////////

template<vtm::traits::StringLike TString,
         vtm::traits::StringLike TView,
         std::signed_integral TInt,
         std::floating_point TFloat,
         FpsFormatFactory TFps>
class __BasicTimecode : public vtm::traits::__reset
                      , public vtm::traits::__implicit_string_overloads<TString, TView>
                      , public vtm::traits::__display<typename vtm::traits::__implicit_string_overloads<TString, TView>::string_view_type>
                      , public vtm::traits::__convert_to_float<__BasicTimecode<TString, TView, TInt, TFloat, TFps>, TFloat>
                      , public vtm::traits::__convert_to_signed<__BasicTimecode<TString, TView, TInt, TFloat, TFps>, TInt>
                      , public vtm::traits::__convert_to_string<__BasicTimecode<TString, TView, TInt, TFloat, TFps>, TString>
{

///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Type Aliases -- 
//
///////////////////////////////////////////////////////////////////////////

public:
    using __my_type     = __BasicTimecode<TString, TView, TInt, TFloat, TFps>;
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
                          const fps_t fps = fps_factory_t::default_value()) -> __BasicTimecode
    {
        VTM_ASSERT(h >= 0, "timecode hours must be greater than or equal to zero");
        VTM_ASSERT(m >= 0, "timecode minutes must be greater than or equal zero");
        VTM_ASSERT(s >= 0, "timecode seconds must be greater than or equal zero");
        VTM_ASSERT(f >= 0, "timecode frames must be greater than or equal zero");

        return __BasicTimecode {
            chunks_to_total_ticks(std::make_tuple(float_type(h), float_type(m), float_type(s), float_type(f)),
                                  __FPSFORMAT_VALUE_TO_FLOAT(fps)),
            fps
        };
    }

    template<vtm::traits::StringConstructible S>
    static auto from_string(const S& tc, const fps_t fps = fps_factory_t::default_value()) -> __BasicTimecode
    {
        const char* msg = "cannot create new timecode object with invalid timecode string";

        if constexpr (std::same_as<string_t, S> || std::same_as<string_view_t, S>){
            VTM_ASSERT(valid_tcstring(tc), msg);
            return __BasicTimecode { tcstring_to_ticks(tc, __FPSFORMAT_VALUE_TO_FLOAT(fps)),
                                     fps };
        }

        else {
            VTM_ASSERT(valid_tcstring(string_t(tc)), msg);
            return __BasicTimecode { tcstring_to_ticks(string_t(tc), __FPSFORMAT_VALUE_TO_FLOAT(fps)), 
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

    auto display() const -> display_t { return basic_timecode_default_display<display_t>(); }

    // TODO: Test that this doesn't stack overflow because of implicit type conversion
    explicit operator string_view_t() const { return string_type(*this); }

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Ctors, Dtors & Assignment --
//
///////////////////////////////////////////////////////////////////////////

public:
    __BasicTimecode() = default;
    ~__BasicTimecode() = default;

    __BasicTimecode(const __BasicTimecode& tc)
        : _value(tc._value)
        , _fps(tc._fps)
    {}

    __BasicTimecode(__BasicTimecode&& tc) noexcept
    {
        this->_value = tc._value;
        this->_fps = tc._fps;
        tc._value = {};
        tc._fps = {};
    }

    __BasicTimecode& operator=(const __BasicTimecode& tc)
    {
        this->_value = tc._value;
        this->_fps = tc._fps;
        return *this;
    }

    __BasicTimecode& operator=(__BasicTimecode&& tc) noexcept
    {
        this->_value = tc._value;
        this->_fps = tc._fps;
        tc._value = {};
        tc._fps = {};
        return *this;
    }

    template<TimecodePrimitive V>
    explicit __BasicTimecode(const V value, const fps_t fps = fps_factory_t::default_value()) noexcept
        : _value(value)
        , _fps(fps)
    {}

    explicit constexpr __BasicTimecode(const string_view_t& tc)
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
    void set_value(const __BasicTimecode& value) noexcept
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
//           -- @SECTION Implicit Type Conversions --
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
    __BasicTimecode& operator=(const V rhs)
    {
        this->set_value(rhs);
        return *this;
    }

    __BasicTimecode& operator+=(const auto& rhs)
    {
        this->set_value(*this + rhs);
        return *this;
    }

    __BasicTimecode& operator-=(const auto& rhs)
    {
        this->set_value(*this - rhs);
        return *this;
    }

    template<TimecodePrimitive V>
    friend __BasicTimecode operator+(const __BasicTimecode& lhs, const V rhs)
    {
        __BasicTimecode tmp{ lhs };

        tmp.set_value(std::min(std::numeric_limits<float_type>::max(),
                      tmp._value + rhs));

        return tmp;
    }

    template<TimecodePrimitive V>
    friend V operator+(const V lhs, const __BasicTimecode& rhs)
    {
        return lhs + rhs._value;
    }

    template<TimecodePrimitive V>
    friend __BasicTimecode operator-(const __BasicTimecode& lhs, const V rhs)
    {
        __BasicTimecode tmp{ lhs };

        tmp.set_value(std::max(std::numeric_limits<float_type>::min(),
                      tmp._value - rhs));

        return tmp;
    }

    template<TimecodePrimitive V>
    friend V operator-(const V lhs, const __BasicTimecode& rhs)
    {

        return lhs - rhs._value;
    }

    friend __BasicTimecode operator+(const __BasicTimecode& lhs, const __BasicTimecode& rhs)
    {
        __BasicTimecode tmp { lhs };

        tmp.set_value(std::min(std::numeric_limits<float_type>::max(),
                      tmp._value + rhs._value));

        return tmp;
    }

    friend __BasicTimecode operator-(const __BasicTimecode& lhs, const __BasicTimecode& rhs)
    {
        __BasicTimecode tmp { lhs };

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
    bool operator==(const __BasicTimecode& rhs) const
    {
        return this->_value == rhs._value && this->_fps == rhs._fps;
    }

    auto operator<=>(const __BasicTimecode& rhs) const
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
        static_assert(Index < 2, "index out of bounds for __BasicTimecode");
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
    // @SECTION __BasicTimecode Data Members
    float_type _value = 0.0;
    fps_t _fps = fps_factory_t::default_value();

};

///////////////////////////////////////////////////////////////////////////

} // @END OF namespace vtm::chrono::internal

///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Public Interface vtm::chrono --
//
///////////////////////////////////////////////////////////////////////////

namespace vtm::chrono {

using fps = internal::__FPSFormat<float64_t, int64_t>;
using timecode = internal::__BasicTimecode<std::string,
                                           std::string_view,
                                           int64_t,
                                           float64_t,
                                           fps>;

} // @END OF namespace vtm::chrono

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Public Interface vtm --
//
///////////////////////////////////////////////////////////////////////////

namespace vtm {

// @SECTION: VTM timecode object aliases
using timecode = chrono::timecode;
using tcfloat_t = typename chrono::timecode::float_type;
using tcint_t = typename chrono::timecode::signed_type;

// @SECTION: VTM FPS factory object aliases
using fps = chrono::fps;
using fpsfloat_t = typename chrono::fps::float_type;
using fpsint_t = typename chrono::fps::int_type;

} // @END OF namespace vtm

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Extensions to namespace std --
//
///////////////////////////////////////////////////////////////////////////

namespace std {

// @SECTION: __BasicTimecode std::tuple specialization
template<>
struct tuple_size<vtm::timecode> : std::integral_constant<std::size_t, 2> {};

template<std::size_t Index>
struct tuple_element<Index, vtm::timecode>
    : tuple_element<Index,
                    tuple<typename vtm::timecode::__element1_type,
                          typename vtm::timecode::__element2_type >>
{};

} // @END OF namespace std

///////////////////////////////////////////////////////////////////////////
