// Copyright (C) Stefan Olivier
// <https://stefanolivier.com>
// ----------------------------
// Description: Header for Verbatim timecode library

#pragma once

///////////////////////////////////////////////////////////////////////////

// Library headers
#include "timecode_common.hpp"
#include "timecode_int.hpp"
#include "timecode_float.hpp"
#include <type_traits>

///////////////////////////////////////////////////////////////////////////

#ifndef VTM_TIMECODE_MACROS
#define VTM_TIMECODE_MACROS

#endif // @END OF VTM_TIMECODE_MACROS

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Public Interface vtm::chrono --
//
///////////////////////////////////////////////////////////////////////////

namespace vtm::chrono {

using fps = internal::__FPSFormat<float64_t, int64_t>;

using timecode = internal::__BasicTimecodeInt<int64_t,
                                              float64_t,
                                              std::string,
                                              std::string_view,
                                              fps>;

using f64timecode = internal::__BasicTimecodeFloat<std::string,
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
using f64timecode = chrono::f64timecode;

// @SECTION: VTM FPS factory object aliases
using fps = chrono::fps;
using fpsfloat_t = typename chrono::fps::float_type;
using fpsint_t = typename chrono::fps::signed_type;
using fpsuint_t = typename chrono::fps::unsigned_type;

} // @END OF namespace vtm

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//           -- @SECTION Extensions to namespace std --
//
///////////////////////////////////////////////////////////////////////////

namespace std {
// @SECTION: __BasicTimecodeInt std::tuplee specialization
template<>
struct tuple_size<vtm::timecode> : std::integral_constant<std::size_t, 6>{}; // TODO: constant for magic number

template<std::size_t Index>
struct tuple_element<Index, vtm::timecode>
    : tuple_element<Index,
                    tuple<typename vtm::timecode::__element1_type,
                          typename vtm::timecode::__element2_type,
                          typename vtm::timecode::__element3_type,
                          typename vtm::timecode::__element4_type,
                          typename vtm::timecode::__element5_type,
                          typename vtm::timecode::__element6_type >>
{};

// @SECTION: __BasicTimecodeFloat std::tuple specialization
template<>
struct tuple_size<vtm::f64timecode> : std::integral_constant<std::size_t, 2>{}; // TODO: constant for magic number

template<std::size_t Index>
struct tuple_element<Index, vtm::f64timecode>
    : tuple_element<Index,
                    tuple<typename vtm::f64timecode::__element1_type,
                          typename vtm::f64timecode::__element2_type >>
{};

} // @END OF namespace std

///////////////////////////////////////////////////////////////////////////
