// Copyright (C) Stefan Olivier
// <https://stefanolivier.com>
// ----------------------------
// Description: Timecode library templates for integral point implementation

#pragma once

///////////////////////////////////////////////////////////////////////////

// Standard headers
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

#define TCSTRING_SIZE (TC_GROUP_WIDTH * TC_TOTAL_GROUPS)
#define TCSTRING_COLON_DEFAULT ":"
#define TCSTRING_COLON_DROPFRAME ";"
#define TCSTRING_HRS_START 0
#define TCSTRING_MINS_START (TC_GROUP_WIDTH * 1)
#define TCSTRING_SECS_START (TC_GROUP_WIDTH * 2)
#define TCSTRING_FRAMES_START (TC_GROUP_WIDTH * 3)
#define TCSTRING_SUBFRAMES_START (TC_GROUP_WIDTH * 4)

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

#define TCSCALAR_1HR_IN_SUBFRAMES (60 * 60 * TCSCALAR_SUBFRAMES_PER_FRAMES)
#define TCSCALAR_1MIN_IN_SUBFRAMES (60 * TCSCALAR_SUBFRAMES_PER_FRAMES)
#define TCSCALAR_1SEC_IN_SUBFRAMES TCSCALAR_SUBFRAMES_PER_FRAMES

// TODO: Make this accept variable size
#define UNWRAP_TCVALUES(p, m, n) p.m[0], p.m[1], p.m[2], p.m[3], p.m[4]

///////////////////////////////////////////////////////////////////////////
//
//  @SECTION Helpers for __BasicTimecodeInt
//
///////////////////////////////////////////////////////////////////////////

template<std::integral... Is, std::size_t Size = sizeof...(Is)>
static constexpr auto __init_tick_groups(Is... is)
{
    return std::tuple { [=](std::unsigned_integral auto fps) -> std::uint64_t {
        if (is == 0) return TCSCALAR_SUBFRAMES_PER_FRAMES;
        else if (is == -1) return 1; 
        else return is * fps * TCSCALAR_SUBFRAMES_PER_FRAMES;
    } ... };
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
    using __element1_type = float_t;
    using __element2_type = fps_t;
    using flags_t         = std::bitset<TC_FLAGS_SIZE>;
    using scalar_t        = std::uint8_t;

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  @SECTION Static Data Members
//
///////////////////////////////////////////////////////////////////////////

public:
    static constexpr std::tuple __tick_groups = __init_tick_groups(
        TCSCALAR_1HR_IN_SUBFRAMES,
        TCSCALAR_1MIN_IN_SUBFRAMES,
        TCSCALAR_1SEC_IN_SUBFRAMES,
        0,
        -1
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
        auto fps_value = fps_factory_t::to_signed(this->_fps);
        return vtm::utility::accumulate<signed_type>(
                static_cast<signed_type>(this->at<TCSCALAR_HRS_START>()) * 60 * 60 * fps_value,
                static_cast<signed_type>(this->at<TCSCALAR_MINS_START>()) * 60 * fps_value,
                static_cast<signed_type>(this->at<TCSCALAR_SECS_START>()) * fps_value,
                static_cast<signed_type>(this->at<TCSCALAR_FRAMES_START>())
        ) * TCSCALAR_SUBFRAMES_PER_FRAMES + static_cast<signed_type>(this->at<TCSCALAR_SUBFRAMES_START>());
    }

    operator unsigned_type() const
    {
        VTM_TODO("not implemented");
    }

    operator float_type() const
    {
        VTM_TODO("not implemented");
    }

    operator string_type() const
    {
        VTM_TODO("not implemented");
    }

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION Accessors & Mutators --
//
///////////////////////////////////////////////////////////////////////////

public:
    // TODO: Frame rate aware set_value()
    void set_value(const __BasicTimecodeInt& value)
    {
        this->_values = value._values;
    }

    template<TimecodePrimitive V>
    void set_value(const V value) noexcept
    {
        VTM_TODO("not implemented");
    }

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
        const auto [ value, remainder ] = this->reduce<TCSCALAR_HRS_START>(hours);
        // TODO: Handle overflow if (remainder > 0) 
        this->_values[TCSCALAR_HRS_START] = value / (60 * 60) / fps_factory_t::to_unsigned(this->_fps) / TCSCALAR_SUBFRAMES_PER_FRAMES;
    }

    scalar_t hours() const noexcept
    {
        return this->_values[TCSCALAR_HRS_START];
    }

    template<std::integral T>
    void set_minutes(T minutes)
    {
        const auto [ value, remainder ] = this->reduce<TCSCALAR_HRS_START>(minutes);
        if (remainder > 0) this->set_hours(remainder);
        this->_values[TCSCALAR_MINS_START] = value / 60 / fps_factory_t::to_unsigned(this->_fps) / TCSCALAR_SUBFRAMES_PER_FRAMES;
    }

    scalar_t minutes() const noexcept
    {
        return this->_values[TCSCALAR_MINS_START];
    }

    template<std::integral T>
    void set_seconds(T seconds)
    {
        const auto [ value, remainder ] = this->reduce<TCSCALAR_HRS_START>(seconds);
        if (remainder > 0) this->set_minutes(remainder);
        this->_values[TCSCALAR_SECS_START] = value / fps_factory_t::to_unsigned(this->_fps) / TCSCALAR_SUBFRAMES_PER_FRAMES;
    }

    scalar_t seconds() const noexcept
    {
        return this->_values[TCSCALAR_SECS_START];
    }

    template<std::integral T>
    void set_frames(T frames)
    {
        const auto [ value, remainder ] = this->reduce<TCSCALAR_HRS_START>(frames);
        if (remainder > 0) this->set_seconds(remainder);
        this->_values[TCSCALAR_FRAMES_START] = value / fps_factory_t::to_unsigned(this->_fps) / TCSCALAR_SUBFRAMES_PER_FRAMES;
    }

    scalar_t frames() const noexcept
    {
        return this->_values[TCSCALAR_FRAMES_START];
    }

    template<std::integral T>
    void set_subframes(T subframes)
    {
        const auto [ value, remainder ] = this->reduce<TCSCALAR_HRS_START>(subframes);
        if (remainder > 0) this->set_frames(remainder);
        this->_values[TCSCALAR_SUBFRAMES_START] = value;
    }

    scalar_t subframes() const noexcept
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
    constexpr auto reduce(T value) const
    {
        static_assert(Index < TC_TOTAL_GROUPS, "index for static data member tick_groups must be less than TC_TOTAL_GROUPS");

        const std::uint64_t tick_size = std::get<Index>(__BasicTimecodeInt::__tick_groups)(fps_factory_t::to_unsigned(this->_fps));
        VTM_ASSERT(tick_size > 0, "tick_size evaluated to 0, which would result in undefined behaviour with division by zero");
        
        std::uint64_t v = value / tick_size;
        std::uint64_t r = value % tick_size;

        if (value >= tick_size && Index < TC_TOTAL_GROUPS) {
            const auto [ v_, r_ ] = this->reduce<Index + 1>(value);
            v = v_;
            r = r_;
        }

        return std::make_tuple(v, r);
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

#undef TCSTRING_SIZE
#undef TCSTRING_COLON_DEFAULT
#undef TCSTRING_COLON_DROPFRAME
#undef TCSTRING_HRS_START
#undef TCSTRING_MINS_START
#undef TCSTRING_SECS_START
#undef TCSTRING_FRAMES_START
#undef TCSTRING_SUBFRAMES_START

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

} // @END OF namespace vtm::chrono::internal

///////////////////////////////////////////////////////////////////////////
