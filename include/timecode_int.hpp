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

#define TCSTRING_GROUP_WIDTH 2
#define TCSTRING_TOTAL_GROUPS 4
#define TCSTRING_SIZE (TCSTRING_GROUP_WIDTH * TCSTRING_TOTAL_GROUPS)
#define TCSTRING_COLON_DEFAULT ":"
#define TCSTRING_COLON_DROPFRAME ";"
#define TCSTRING_HRS_START 0
#define TCSTRING_MINS_START (TCSTRING_GROUP_WIDTH * 1)
#define TCSTRING_SECS_START (TCSTRING_GROUP_WIDTH * 2)
#define TCSTRING_FRAMES_START (TCSTRING_GROUP_WIDTH * 3)
#define TC_FLAGS_SIZE 8

// TODO: concept & type trait to get unsigned and signed types
template<std::integral TInt,
         std::floating_point TFloat,
         vtm::traits::StringLike TString,
         vtm::traits::StringLike TView,
         FpsFormatFactory TFps>
class __BasicTimecodeInt : public vtm::traits::__reset
                         , public vtm::traits::__implicit_string_overloads<TString, TView>
                         , public vtm::traits::__display<typename vtm::traits::__implicit_string_overloads<TString, TView>::string_view_type>
                         , public vtm::traits::__convert_to_signed<__BasicTimecodeInt<TInt, TFloat, TString, TView, TFps>, TInt>
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
    using signed_type     = typename vtm::traits::__convert_to_signed<__my_type, TInt>::signed_type;
    using float_type      = typename vtm::traits::__convert_to_float<__my_type, TFloat>::float_type;
    using string_type     = typename vtm::traits::__convert_to_string<__my_type, string_t>::string_type;
    using char_t          = vtm::traits::string_char_type_t<string_t>;
    using display_t       = string_view_t;
    using fps_factory_t   = TFps;
    using fps_t           = typename TFps::type;
    using __element1_type = float_t;
    using __element2_type = fps_t;
    using flags_t         = std::bitset<TC_FLAGS_SIZE>;

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION Ctors, Dtors & Assignment --
//
///////////////////////////////////////////////////////////////////////////

public:
    __BasicTimecodeInt() = default;
    ~__BasicTimecodeInt() = default;

    __BasicTimecodeInt(const __BasicTimecodeInt& tc)
        : _fps(tc._fps)
        , _flags(tc._flags)
        , _values(tc._values)
    {}

    __BasicTimecodeInt(__BasicTimecodeInt&& tc) noexcept
        : _fps(tc._fps)
        , _flags(tc._flags)
        , _values(tc._values)
    {
        tc._fps = {};
        tc._flags = {};
        tc._values = {};
    }

    __BasicTimecodeInt& operator=(const __BasicTimecodeInt& tc)
    {
        this->_fps = tc._fps;
        this->_flags = tc._flags;
        this->_values = tc._values;
        return *this;
    }

    __BasicTimecodeInt& operator=(__BasicTimecodeInt&& tc) noexcept
    {
        this->_fps = tc._fps;
        this->_flags = tc._flags;
        this->_values = tc._values;
        tc._fps = {};
        tc._flags = {};
        tc._values = {};
        return *this;
    }

    template<TimecodePrimitive V>
    explicit __BasicTimecodeInt(const V value, const fps_t fps = fps_factory_t::default_value()) noexcept
        : _fps(fps)
        , _flags(0)
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
        for (std::size_t i = 0; i < TCSTRING_SIZE; ++i) {
            this->_values[i] = 0;
        }
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
// -- @SECTION Private Members --
//
///////////////////////////////////////////////////////////////////////////

private:
    fps_t _fps = fps_factory_t::default_value();
    flags_t _flags = 0;
    std::uint8_t _values[TCSTRING_SIZE] = {0, 0, 0, 0};
};

#undef TCSTRING_GROUP_WIDTH
#undef TCSTRING_TOTAL_GROUPS
#undef TCSTRING_SIZE
#undef TCSTRING_COLON_DEFAULT
#undef TCSTRING_COLON_DROPFRAME
#undef TCSTRING_HRS_START
#undef TCSTRING_MINS_START
#undef TCSTRING_SECS_START
#undef TCSTRING_FRAMES_START
#undef TC_FLAGS_SIZE

///////////////////////////////////////////////////////////////////////////

} // @END OF namespace vtm::chrono::internal

///////////////////////////////////////////////////////////////////////////
