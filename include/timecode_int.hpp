// Copyright (C) Stefan Olivier
// <https://stefanolivier.com>
// ----------------------------
// Description: Timecode library templates for integral point implementation

#pragma once

///////////////////////////////////////////////////////////////////////////

// Standard headers
#include <algorithm>
#include <bitset>
#include <concepts>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <type_traits>
#include <utility>

// Library headers
#include "errors.hpp"
#include "timecode_common.hpp"
#include "traits.hpp"
#include "utility.hpp"

///////////////////////////////////////////////////////////////////////////

#ifndef VTM_TIMECODE_INT_MACROS
#define VTM_TIMECODE_INT_MACROS



#endif // @END OF VTM_TIMECODE_INT_MACROS

///////////////////////////////////////////////////////////////////////////

namespace vtm::chrono::internal {

///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION __BasicTimecodeInt --
//
///////////////////////////////////////////////////////////////////////////

#define TC_GROUP_WIDTH 2
#define TC_TOTAL_GROUPS 5
#define TC_FLAGS_SIZE 8

#define TCSTRING_GROUP_DEFAULT { '0', '0' }
#define TCSTRING_COLON_DEFAULT ':'
#define TCSTRING_COLON_DROPFRAME ';'
#define TCSTRING_HRS_START 0
#define TCSTRING_MINS_START (TC_GROUP_WIDTH * 1)
#define TCSTRING_SECS_START (TC_GROUP_WIDTH * 2)
#define TCSTRING_FRAMES_START (TC_GROUP_WIDTH * 3)
#define TCSTRING_SUBFRAMES_START (TC_GROUP_WIDTH * 4)
#define TCSTRING_SIZE ((TC_GROUP_WIDTH * TC_TOTAL_GROUPS) + (TC_TOTAL_GROUPS - 1))
#define TCSTRING_CHAR_OFFSET 48

#define TCSCALAR_HRS_MAX 60
#define TCSCALAR_MINS_MAX 60
#define TCSCALAR_SECS_MAX 60
#define TCSCALAR_FRAMES_MAX 60
#define TCSCALAR_SUBFRAMES_MAX 100
#define TCSCALAR_HRS_START 0
#define TCSCALAR_MINS_START 1
#define TCSCALAR_SECS_START 2
#define TCSCALAR_FRAMES_START 3
#define TCSCALAR_SUBFRAMES_START 4
#define TCSCALAR_SUBFRAMES_PER_FRAMES 100

#define TCSCALAR_HRS_TICKS (60 * 60)
#define TCSCALAR_MINS_TICKS 60
#define TCSCALAR_SECS_TICKS 1

#define TCSCALAR_1HR_IN_SUBFRAMES (TCSCALAR_HRS_TICKS * TCSCALAR_SUBFRAMES_PER_FRAMES)
#define TCSCALAR_1MIN_IN_SUBFRAMES (TCSCALAR_MINS_TICKS * TCSCALAR_SUBFRAMES_PER_FRAMES)
#define TCSCALAR_1SEC_IN_SUBFRAMES (TCSCALAR_SECS_TICKS * TCSCALAR_SUBFRAMES_PER_FRAMES)

#define TCGRP_SCALAR_START 0
#define TCGRP_SCALAR_MIN 1
#define TCGRP_SCALAR_MAX 2
#define TCGRP_SCALAR_IN_SUBFRAMES 3
#define TCGRP_STRING_START 4
#define TICK_GROUPS = __my_type::__tick_groups
#define GET_TICK_GRPS(inner, outer) std::get<outer>(std::get<inner>(TICK_GROUPS))

// TODO: Make this accept variable size
#define UNWRAP_TCVALUES(p, m, n) p.m[0], p.m[1], p.m[2], p.m[3], p.m[4]

// TODO: Make this accept variable size
#define TCSTRING_DEFAULT_INITIALIZER {    \
        '0', '0', TCSTRING_COLON_DEFAULT, \
        '0', '0', TCSTRING_COLON_DEFAULT, \
        '0', '0', TCSTRING_COLON_DEFAULT, \
        '0', '0', TCSTRING_COLON_DEFAULT, \
        '0', '0'                          \
    }

///////////////////////////////////////////////////////////////////////////
//
//  @SECTION Static helpers for __BasicTimecodeInt
//
///////////////////////////////////////////////////////////////////////////

template<typename... Ts, std::size_t... Is>
static constexpr auto __init_tick_groups(std::tuple<Ts...> is, std::index_sequence<Is...> seq)
{
    return std::tuple {
        std::tuple {
            std::get<Is>(is)[0],
            std::get<Is>(is)[1],
            std::get<Is>(is)[2],
            std::get<Is>(is)[4],

            [=](std::unsigned_integral auto fps) -> std::uint64_t {
                if (std::get<Is>(is)[3] == 0) return TCSCALAR_SUBFRAMES_PER_FRAMES;
                else if (std::get<Is>(is)[3] == -1) return 1; 
                return std::get<Is>(is)[3] * fps;
            },

            [=]<typename T>(T value) {
                T str[TC_GROUP_WIDTH] = TCSTRING_GROUP_DEFAULT;

                /* if (value < std::get<Is>(is)[2]) { */
                /*     vtm::utility::to_string(value, str); */
                /* } */

                return 'a';
            }
        } ...
    };
}

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////

// TODO: concept & type trait to get unsigned and signed types
template<std::integral TInt,
         std::floating_point TFloat,
         vtm::traits::StringLike TString,
         vtm::traits::StringLike TView,
         FpsFormatFactory TFps>
class __BasicTimecodeInt : public vtm::traits::__reset
                         , public vtm::traits::__implicit_string_overloads<TString, TView>
                         , public vtm::traits::__display<typename vtm::traits::__implicit_string_overloads<TString, TView>::string_view_type>
                         , public vtm::traits::__convert_to_signed<__BasicTimecodeInt<TInt, TFloat, TString, TView, TFps>, vtm::traits::to_signed_t<TInt>>
                         , public vtm::traits::__convert_to_unsigned<__BasicTimecodeInt<TInt, TFloat, TString, TView, TFps>, vtm::traits::to_unsigned_t<TInt>>
                         , public vtm::traits::__convert_to_float<__BasicTimecodeInt<TInt, TFloat, TString, TView, TFps>, TFloat>
                         , public vtm::traits::__convert_to_string<__BasicTimecodeInt<TInt, TFloat, TString, TView, TFps>, TString>
{

///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION Type Aliases --
//
///////////////////////////////////////////////////////////////////////////

public:
    using __my_type       = __BasicTimecodeInt<TInt, TFloat, TString, TView, TFps>;
    using string_t        = typename vtm::traits::__implicit_string_overloads<TString, TView>::string_type;
    using string_view_t   = typename vtm::traits::__implicit_string_overloads<TString, TView>::string_view_type;
    using signed_type     = typename vtm::traits::__convert_to_signed<__my_type, vtm::traits::to_signed_t<TInt>>::signed_type;
    using unsigned_type   = typename vtm::traits::__convert_to_unsigned<__my_type, vtm::traits::to_unsigned_t<TInt>>::unsigned_type;
    using float_type      = typename vtm::traits::__convert_to_float<__my_type, TFloat>::float_type;
    using string_type     = typename vtm::traits::__convert_to_string<__my_type, string_t>::string_type;
    using char_t          = vtm::traits::string_char_type_t<string_t>;
    using display_t       = string_view_t;
    using fps_factory_t   = TFps;
    using fps_t           = typename TFps::type;
    using flags_t         = std::bitset<TC_FLAGS_SIZE>;
    using scalar_t        = std::uint8_t;

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  @SECTION Private Static Data Members
//
///////////////////////////////////////////////////////////////////////////

private:
    static constexpr std::tuple __tick_groups = __init_tick_groups(
        std::tuple{
            std::array { TCSCALAR_HRS_START,       0, TCSCALAR_HRS_MAX,       TCSCALAR_1HR_IN_SUBFRAMES,  TCSTRING_HRS_START       },
            std::array { TCSCALAR_MINS_START,      0, TCSCALAR_MINS_MAX,      TCSCALAR_1MIN_IN_SUBFRAMES, TCSTRING_MINS_START      },
            std::array { TCSCALAR_SECS_START,      0, TCSCALAR_SECS_MAX,      TCSCALAR_1SEC_IN_SUBFRAMES, TCSTRING_SECS_START      },
            std::array { TCSCALAR_FRAMES_START,    0, TCSCALAR_FRAMES_MAX,    0                         , TCSTRING_FRAMES_START    },
            std::array { TCSCALAR_SUBFRAMES_START, 0, TCSCALAR_SUBFRAMES_MAX, -1                        , TCSTRING_SUBFRAMES_START }
        },
        std::make_index_sequence<TC_TOTAL_GROUPS>{}
    );

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION Ctors, Dtors & Assignment --
//
///////////////////////////////////////////////////////////////////////////

public:
    constexpr __BasicTimecodeInt() = default;
    constexpr ~__BasicTimecodeInt() = default;

    constexpr __BasicTimecodeInt(const __BasicTimecodeInt& tc)
        : _fps(tc._fps)
        , _flags(tc._flags)
        , _values{UNWRAP_TCVALUES(tc, _values, TC_TOTAL_GROUPS)}
    {}

    constexpr __BasicTimecodeInt(__BasicTimecodeInt&& tc) noexcept
        : _fps(tc._fps)
        , _flags(tc._flags)
        , _values{UNWRAP_TCVALUES(tc, _values, TC_TOTAL_GROUPS)}
    {
        tc._fps = {};
        tc._flags = {};
        tc._values = {};
    }

    constexpr __BasicTimecodeInt& operator=(const __BasicTimecodeInt& tc)
    {
        *this = __BasicTimecodeInt{ tc };
        return *this;
    }

    constexpr __BasicTimecodeInt& operator=(__BasicTimecodeInt&& tc) noexcept
    {
        *this = __BasicTimecodeInt{ std::move(tc) };
        return *this;
    }

    template<TimecodePrimitive V>
    explicit constexpr __BasicTimecodeInt(const V value,
                                          const fps_t fps = fps_factory_t::default_value()) noexcept
        : _fps(fps)
        , _flags({})
        , _values{}
    {
        VTM_TODO("not implemented");
    }

    explicit constexpr __BasicTimecodeInt(const string_view_t& tc)
    {
        VTM_TODO("not implemented");
    }

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION Virtual Methods --
//
///////////////////////////////////////////////////////////////////////////

public:
    void reset() noexcept
    {
        this->_fps = fps_factory_t::default_value();
        this->_flags = {};
        for (std::size_t i = 0; i < TC_TOTAL_GROUPS; ++i)
            this->_values[i] = 0;
    }

    auto display() const -> display_t
    {
        VTM_TODO("not implemented");
    }

    // TODO: Test that this doesn't stack overflow because of implicit type conversion
    explicit operator string_view_t() const
    {
        return string_type(*this);
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
        return this->implicit_number_conversion_impl<signed_type>(std::make_index_sequence<TC_TOTAL_GROUPS>{});
    }

    operator unsigned_type() const
    {
        return this->implicit_number_conversion_impl<unsigned_type>(std::make_index_sequence<TC_TOTAL_GROUPS>{});
    }

    operator float_type() const
    {
        return this->implicit_number_conversion_impl<float_type>(std::make_index_sequence<TC_TOTAL_GROUPS>{});
    }

    operator string_type() const
    {
        /* VTM_TODO("not implemented"); */

        char_t tc_string[TCSTRING_SIZE] = TCSTRING_DEFAULT_INITIALIZER;
        this->fill_tcstring_array(tc_string, std::make_index_sequence<TC_TOTAL_GROUPS>{});
        string_t str(TCSTRING_SIZE, '\0');

        for (std::size_t i = 0; i < TCSTRING_SIZE; ++i) {
            str[i] = tc_string[i];
        }

        return str;
    }

private:
    template<TimecodePrimitive T, std::size_t... Is>
    constexpr auto implicit_number_conversion_impl(std::index_sequence<Is...> seq) const noexcept -> T
    {
        static_assert(sizeof...(Is) == TC_TOTAL_GROUPS,
                      "index sequence of local input variable \"seq\" has more indexes than this->_values container, which will result in buffer overflow");

        const auto fps_factor = fps_factory_t::to_unsigned(this->_fps);
        return (
            (this->_values[std::get<0>(std::get<Is>(__my_type::__tick_groups))]
             * std::get<3>(std::get<Is>(__my_type::__tick_groups))(fps_factor))
             + ...
        );
    }

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION Accessors & Mutators --
//
///////////////////////////////////////////////////////////////////////////

public:
    template<std::integral V>
    void set_ticks(const V ticks)
    {
        VTM_ASSERT(ticks >= 0, "cannot set ticks to value less than 0");
        this->set_ticks_impl(ticks, std::make_index_sequence<TC_TOTAL_GROUPS>{});
    }

private:
    template<std::integral T, std::size_t... Is>
    constexpr void set_ticks_impl(T ticks, std::index_sequence<Is...> seq)
    {
        static_assert(sizeof...(Is) == TC_TOTAL_GROUPS,
                      "index sequence of local input variable \"seq\" has more indexes than this->_values container, which will result in buffer overflow");

        const unsigned_type fps_factor = fps_factory_t::to_unsigned(this->_fps);
        const std::array indexes = { std::get<0>(std::get<Is>(__my_type::__tick_groups)) ... };
        const std::array factors = { std::get<3>(std::get<Is>(__my_type::__tick_groups))(fps_factor) ... };
        for (const auto& i : indexes) {
            if (ticks >= factors[i]) {
                this->_values[i] = ticks / factors[i];
                ticks %= factors[i];
            }

            else {
                this->_values[i] = 0;
            }
        }

        // TODO: handle remainder of ticks > 0
        this->_values[TCSCALAR_SUBFRAMES_START] += ticks;
    }

public:
    void set_fps(const fps_t fps) noexcept
    {
        this->_fps = fps;
    }

    fps_t fps() const noexcept
    {
        return this->_fps;
    }

    template<std::integral T>
    void set_hours(T hours)
    {
        const auto ticks = this->to_ticks<TCSCALAR_HRS_START>(std::forward<T>(hours));
        this->set_ticks(ticks);
    }

    unsigned_type hours() const noexcept
    {
        return this->_values[TCSCALAR_HRS_START];
    }

    template<std::integral T>
    void set_minutes(T minutes)
    {
        const auto ticks = this->to_ticks<TCSCALAR_MINS_START>(std::forward<T>(minutes));
        this->set_ticks(ticks);
    }

    unsigned_type minutes() const noexcept
    {
        return this->_values[TCSCALAR_MINS_START];
    }

    template<std::integral T>
    void set_seconds(T seconds)
    {
        const auto ticks = this->to_ticks<TCSCALAR_SECS_START>(std::forward<T>(seconds));
        this->set_ticks(ticks);
    }

    unsigned_type seconds() const noexcept
    {
        return this->_values[TCSCALAR_SECS_START];
    }

    template<std::integral T>
    void set_frames(T frames)
    {
        const auto ticks = this->to_ticks<TCSCALAR_FRAMES_START>(std::forward<T>(frames));
        this->set_ticks(ticks);
    }

    unsigned_type frames() const noexcept
    {
        return this->_values[TCSCALAR_FRAMES_START];
    }

    template<std::integral T>
    void set_subframes(T subframes)
    {
        const auto ticks = this->to_ticks<TCSCALAR_SUBFRAMES_START>(std::forward<T>(subframes));
        this->set_ticks(ticks);
    }

    unsigned_type subframes() const noexcept
    {
        return this->_values[TCSCALAR_SUBFRAMES_START];
    }

    bool is_drop_frame() const noexcept
    {
        return fps_factory_t::is_drop_frame(this->_fps);
    }

    bool is_negative() const noexcept
    {
        VTM_TODO("not implemented");
    }

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION Private Helper Methods --
//
///////////////////////////////////////////////////////////////////////////

private:
    template<std::size_t I>
    constexpr scalar_t at() const noexcept
    {
        static_assert(I < TC_TOTAL_GROUPS, "size of index must be smaller than TC_TOTAL_GROUPS");
        return this->_values[I];
    }

    template<std::size_t I, scalar_t value>
    constexpr void set_at()
    {
        static_assert(I < TC_TOTAL_GROUPS, "size of index must be smaller than TC_TOTAL_GROUPS");
        static_assert(std::is_same_v<decltype(value), scalar_t>, "value of input type must be the same as scalar_t");
        static_assert(value >= std::numeric_limits<scalar_t>::min(), "value for hours must greater than or eqauls to scalar_t minimum");

        if constexpr (I == 0) {
            static_assert(value <= TCSCALAR_HRS_MAX, "value for hours must be less than or equals TCSCALAR_HRS_MAX");
        }

        else if constexpr (I == 1) {
            static_assert(value <= TCSCALAR_MINS_MAX, "value for hours must be less than or equals TCSCALAR_MINS_MAX");
        }

        else if constexpr (I == 2) {
            static_assert(value <= TCSCALAR_SECS_MAX, "value for hours must be less than or equals TCSCALAR_SECS_MAX");
        }

        else if constexpr (I == 3) {
            static_assert(value <= TCSCALAR_FRAMES_MAX, "value for hours must be less than or equals TCSCALAR_FRAMES_MAX");
        }

        else if constexpr (I == 4) {
            static_assert(value <= TCSCALAR_SUBFRAMES_MAX, "value for hours must be less than or equals TCSCALAR_SUBFRAMES_MAX");
        }

        this->_values[I] = value; 
    }

    template<std::size_t Index, std::integral T>
    constexpr auto to_ticks(T value) const -> vtm::traits::to_unsigned_t<T>
    {
        if (value <= 0) return 0;
        // TODO: Set negative flag if value < 0 ???

        static_assert(Index < TC_TOTAL_GROUPS, "index for static data member tick_groups must be less than TC_TOTAL_GROUPS");
        const auto [ index, min, max, get_tick_factor, _ ] = std::get<Index>(__my_type::__tick_groups);
        const unsigned_type tick_factor = get_tick_factor(fps_factory_t::to_unsigned(this->_fps));
        VTM_ASSERT(tick_factor > 0, "tick_factor evaluated to 0, which could result in unexpected results with multiplication by zero");

        return value * tick_factor;
    }

    template<std::size_t I, std::size_t Size>
    constexpr void array_set(auto& arr, auto value) const
    {
        static_assert(I < Size, "index for array type is greater than its size, this will result in a buffer overflow");
        arr[I] = value;
    }

    template<std::size_t StrSize, std::size_t... Is>
    constexpr void fill_tcstring_array(char_t(&tcstring)[StrSize], std::index_sequence<Is...> seq) const
    {
        ((array_set<
            std::get<4>(std::get<Is>(__my_type::__tick_groups)),
            TCSTRING_SIZE
        >(tcstring, std::get<4>(std::get<Is>(__my_type::__tick_groups))(std::get<0>(std::get<Is>(__my_type::__tick_groups))))), ... );
    }

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
// -- @SECTION Private Members --
//
///////////////////////////////////////////////////////////////////////////

private:
    fps_t _fps = fps_factory_t::default_value();
    flags_t _flags = {};
    scalar_t _values[TC_TOTAL_GROUPS] = {};
};

///////////////////////////////////////////////////////////////////////////

#undef TC_GROUP_WIDTH
#undef TC_TOTAL_GROUPS
#undef TC_FLAGS_SIZE

#undef TCSTRING_GROUP_DEFAULT
#undef TCSTRING_SIZE
#undef TCSTRING_COLON_DEFAULT
#undef TCSTRING_COLON_DROPFRAME
#undef TCSTRING_HRS_START
#undef TCSTRING_MINS_START
#undef TCSTRING_SECS_START
#undef TCSTRING_FRAMES_START
#undef TCSTRING_SUBFRAMES_START
#undef TCSTRING_CHAR_OFFSET

#undef TCSCALAR_HRS_START
#undef TCSCALAR_MINS_START
#undef TCSCALAR_SECS_START
#undef TCSCALAR_FRAMES_START
#undef TCSCALAR_SUBFRAMES_START
#undef TCSCALAR_SUBFRAMES_PER_FRAMES
#undef TCSCALAR_HRS_MAX
#undef TCSCALAR_MINS_MAX
#undef TCSCALAR_SECS_MAX
#undef TCSCALAR_FRAMES_MAX
#undef TCSCALAR_SUBFRAMES_MAX

#undef TCSCALAR_1HR_IN_SUBFRAMES
#undef TCSCALAR_1MIN_IN_SUBFRAMES
#undef TCSCALAR_1SEC_IN_SUBFRAMES

#undef UNWRAP_TCVALUES
#undef TCSTRING_DEFAULT_INITIALIZER

} // @END OF namespace vtm::chrono::internal

///////////////////////////////////////////////////////////////////////////
