// Stefan Olivier
// Description: IO procedures and coniguration

#pragma once

// Standard library
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <optional>
#include <string>
#include <type_traits>

// Project library
#include "errors.hpp"
#include "io.hpp"
#include "traits.hpp"

// @SECTION: Constants for IO
namespace vtm::io
{
    inline constexpr auto FSTREAM_IN      = std::ios::in                  | std::ios::binary;
    inline constexpr auto FSTREAM_OUT     = std::ios::out                 | std::ios::binary;
    inline constexpr auto FSTREAM_IN_ATE  = std::ios::in  | std::ios::ate | std::ios::binary;
    inline constexpr auto FSTREAM_OUT_ATE = std::ios::out | std::ios::ate | std::ios::binary;

    inline constexpr auto FSTREAM_BEGIN   = std::ios::beg;
    inline constexpr auto FSTREAM_CURRENT = std::ios::cur;
    inline constexpr auto FSTREAM_END     = std::ios::end;
}

// @SECTION: Stream parsing procedures
namespace vtm::io
{
    // TODO: Constraints for template arguments
    template<typename TStream, vtm::traits::StringLike TString = std::string>
    inline auto take_one(TStream& stream) -> std::optional<std::remove_cvref_t<TString>>
    {
        using string_t = std::remove_cvref_t<TString>;

        if (stream) {

            const auto current_pos = stream.tellg();
            stream.seekg(FSTREAM_BEGIN, FSTREAM_END);
            const auto size = stream.tellg();
            stream.seekg(current_pos);
            
            string_t str(1, '\0');
            if (stream.read(&str[0], 1)) {
                stream.seekg(0, FSTREAM_CURRENT);
                return str;
            }
        }

        else {
            VTM_WARN("input file stream is invalid");
        }

        return std::nullopt;
    }

    template<typename TStream, vtm::traits::StringLike TString = std::string, typename... Callables>
    inline auto take_while(TStream& stream, const Callables&... f) -> std::remove_cvref_t<TString>
    {
        using string_t = std::remove_cvref_t<TString>;
        static_assert(sizeof...(Callables) > 0, "please provide at least one invocable object");

        string_t ret = "";
        auto one = take_one(stream);

        while(one) {
            if ((f(*one) && ...))  ret += *one;
            else break;
            one = take_one(stream);
        }

        return ret;
    }

    template<typename TStream, vtm::traits::StringLike TString = std::string, typename... Callables>
    inline auto skip(TStream& stream, const Callables&... f) -> std::remove_cvref_t<TString>
    {
        using string_t = std::remove_cvref_t<TString>;
        static_assert(sizeof...(Callables) > 0);

        string_t ret = "";
        auto one = take_one(stream);

        while(one) {
            if ((f(*one) && ...))  ret += *one;
            one = take_one(stream);
        }

        return ret;
    }
}














