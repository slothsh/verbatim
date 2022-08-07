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
#include <limits>
#include <type_traits>

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
//  -- @SECTION Ctors, Dtors & Assignment --
//
///////////////////////////////////////////////////////////////////////////

public:
    constexpr __BasicTimecodeInt() = default;
    constexpr ~__BasicTimecodeInt() = default;

    constexpr __BasicTimecodeInt(const __BasicTimecodeInt& tc)
        : _fps(tc._fps)
        , _flags(tc._flags)
        , _values(tc._values)
    {}

    constexpr __BasicTimecodeInt(__BasicTimecodeInt&& tc) noexcept
        : _fps(tc._fps)
        , _flags(tc._flags)
        , _values(tc._values)
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
        return signed_type{vtm::utility::accumulate(
                this->at<0>(),
                this->at<1>(),
                this->at<2>(),
                this->at<3>()
        )} * TCSCALAR_SUBFRAMES_PER_FRAMES;
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

    template<std::unsigned_integral T>
    void set_hours(T hrs)
    {
        VTM_ERROR_IF(hrs > TCSCALAR_HRS_MAX,
                     "specified value for hours is larger than maximum allowed value");

        this->_values[TCSCALAR_HRS_START] = hrs;
    }

    scalar_t hours() const noexcept
    {
        return this->_values[TCSCALAR_HRS_START];
    }

    template<std::unsigned_integral T>
    void set_minutes(T mins)
    {
        VTM_ERROR_IF(mins > TCSCALAR_MINS_MAX,
                     "specified value for minutes is larger than maximum allowed value");

        this->_values[TCSCALAR_MINS_START] = mins;
    }

    scalar_t minutes() const noexcept
    {
        return this->_values[TCSCALAR_MINS_START];
    }

    template<std::unsigned_integral T>
    void set_seconds(T secs)
    {
        VTM_ERROR_IF(secs > TCSCALAR_SECS_MAX,
                     "specified value for seconds is larger than maximum allowed value");

        this->_values[TCSCALAR_SECS_START] = secs;
    }

    scalar_t seconds() const noexcept
    {
        return this->_values[TCSCALAR_SECS_START];
    }

    template<std::unsigned_integral T>
    void set_frames(T frames)
    {
        VTM_ERROR_IF(frames > TCSCALAR_FRAMES_MAX,
                     "specified value for frames is larger than maximum allowed value");

        this->_values[TCSCALAR_FRAMES_START] = frames;
    }

    scalar_t frames() const noexcept
    {
        return this->_values[TCSCALAR_FRAMES_START];
    }

    template<std::unsigned_integral T>
    void set_subframes(T subframes)
    {
        VTM_ERROR_IF(subframes > TCSCALAR_SUBFRAMES_MAX,
                     "specified value for subframes is larger than maximum allowed value");

        this->_values[TCSCALAR_SUBFRAMES_START] = subframes;
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
        static_assert(I < TC_TOTAL_GROUPS, "size of index must be smaller that TC_TOTAL_GROUPS");
        return this->_values[I];
    }

    template<std::size_t I>
    constexpr void set_at()
    {
        
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

} // @END OF namespace vtm::chrono::internal

///////////////////////////////////////////////////////////////////////////
