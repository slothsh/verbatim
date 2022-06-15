// Stefan Olivier
// Description: Error handling

# pragma once

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

namespace vtm::errors
{
    using ERROR_TYPE = int;
    enum SIGNAL : ERROR_TYPE
    {
        SUCCESS,
        FAIL,
        FAIL_PANIC,
        FAIL_ASSERT,
        UNKNOWN
    };

    namespace internal
    {
        template<typename Tstring>
        inline auto signal_msg(ERROR_TYPE errcode) -> std::remove_cvref_t<Tstring>
        {
            constexpr std::string_view default_msg{ "unknown program failure" };
            switch (errcode) {
                case SIGNAL::SUCCESS:     return "exited successfully";
                case SIGNAL::FAIL:        return "program execution failed";
                case SIGNAL::FAIL_PANIC:  return "program panicked during execution";
                case SIGNAL::FAIL_ASSERT: return "failed assertion caused execution to be terminated";
                default:                  return default_msg;
            }

            return default_msg;
        }
    }
}

// Error and diagnostic macros
#ifndef VTM_ERROR_MACROS
#define VTM_ERROR_MACROS

#define __signal_msg(s) vtm::errors::internal::signal_msg<std::string_view>(s)
#define __diagmsg_prequit(program, signal) fmt::print(stderr,                                  \
                                                      "\n{}: program exited with signal: {}\n" \
                                                      "DESCRIPTION: {}\n\n",                   \
                                                      program, signal, __signal_msg(signal))

#define __diagmsg_noquit(prefix, msg) (fmt::print(stderr, "{}: {}:({}): {}: {}\n", prefix, __FILE__, __LINE__, __func__, msg))

#define __diagmsg_quit(prefix, msg, signal) __diagmsg_noquit(prefix, msg);    \
                                            (std::exit(signal))

#define __diagmsg_panic(msg) __diagmsg_noquit("FATAL", msg); \
                             (std::abort())
 
#define __diagmsg_assert(prefix, expr, msg) if (!(expr)) { __diagmsg_prequit("APPLICATION", vtm::errors::SIGNAL::FAIL_ASSERT); \
                                                           __diagmsg_noquit(prefix, "Assertion failed with: "#expr"\n"#msg);   \
                                                           std::abort();                                                       \
                                                         }

#define VTM_ASSERT(expr, msg) __diagmsg_assert("FATAL", expr, msg)
#define VTM_ERROR(msg)  __diagmsg_noquit("ERROR", msg)
#define VTM_PANIC(msg)  __diagmsg_prequit("APPLICATION", vtm::errors::SIGNAL::FAIL_PANIC); __diagmsg_panic(msg)
#define VTM_TODO(msg)   __diagmsg_prequit("APPLICATION", vtm::errors::SIGNAL::FAIL); __diagmsg_quit("TODO", msg, vtm::errors::SIGNAL::FAIL)
#define VTM_WARN(msg)   __diagmsg_noquit("WARNING", msg)

#endif
























