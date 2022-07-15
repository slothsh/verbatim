// Stefan Olivier
// Description: Error handling

# pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Standard headers
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <type_traits>

// Library headers
#include <fmt/core.h>
#include <fmt/format.h>

// Project headers
#include "traits.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace vtm::errors {

// @SECTION Error Signals

using error_t = int;
enum Signal : error_t
{
    success,
    fail,
    fail_panic,
    fail_assert,
    unknown
};

} // @END OF namespace vtm::errors

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace vtm::errors::internal
{

// @SECTION Error Functions

template<typename Tstring>
inline auto signal_msg(error_t errcode) -> std::remove_cvref_t<Tstring>
{
    constexpr std::string_view default_msg{ "unknown program failure" };
    switch (errcode) {
        case Signal::success:     return "exited successfully";
        case Signal::fail:        return "program execution failed";
        case Signal::fail_panic:  return "program panicked during execution";
        case Signal::fail_assert: return "failed assertion caused execution to be terminated";
        default:                  return default_msg;
    }

    return default_msg;
}

} // @END OF namespace vtm::errors::internal

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// @SECTION Error and Diagnostic Macros
#ifndef VTM_ERROR_MACROS
#define VTM_ERROR_MACROS

#define __SIGNAL_MSG(s) vtm::errors::internal::signal_msg<std::string_view>(s)
#define __DIAGMSG_PREQUIT(program, signal) fmt::print(stderr,                                  \
                                                      "\n{}: program exited with signal: {}\n" \
                                                      "DESCRIPTION: {}\n\n",                   \
                                                      program, signal, __SIGNAL_MSG(signal))

#define __DIAGMSG_NOQUIT(prefix, msg) (fmt::print(stderr, "{}: {}:({}): {}: {}\n", prefix, __FILE__, __LINE__, __func__, msg))

#define __DIAGMSG_QUIT(prefix, msg, signal) __DIAGMSG_NOQUIT(prefix, msg);    \
                                            (std::exit(signal))

#define __DIAGMSG_PANIC(msg) __DIAGMSG_NOQUIT("FATAL", msg); \
                             (std::abort())
 
#define __DIAGMSG_ASSERT(prefix, expr, msg) if (!(expr)) { __DIAGMSG_PREQUIT("APPLICATION", vtm::errors::Signal::fail_assert); \
                                                           __DIAGMSG_NOQUIT(prefix, "Assertion failed with: "#expr"\n"#msg);   \
                                                           std::abort();                                                       \
                                                         }

#define VTM_ASSERT(expr, msg) __DIAGMSG_ASSERT("FATAL", expr, msg)
#define VTM_ERROR(msg)  __DIAGMSG_NOQUIT("ERROR", msg)
#define VTM_PANIC(msg)  __DIAGMSG_PREQUIT("APPLICATION", vtm::errors::Signal::fail_panic); __DIAGMSG_PANIC(msg)
#define VTM_TODO(msg)   __DIAGMSG_PREQUIT("APPLICATION", vtm::errors::Signal::fail); __DIAGMSG_QUIT("TODO", msg, vtm::errors::Signal::fail)
#define VTM_WARN(msg)   __DIAGMSG_NOQUIT("WARNING", msg)

#endif // @END OF VTM_ERROR_MACROS

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
