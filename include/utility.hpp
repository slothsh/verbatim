// Stefan Olivier
// Description: EDL class object for parsing EDL files

#pragma once

// Standard library
#include <algorithm>
#include <array>
#include <cstdint>
#include <concepts>
#include <string>
#include <string_view>
#include <type_traits>
#include <tuple>
#include <utility>

// Third-party library
#include <fmt/format.h>

// Project library
#include "macros.hpp"
#include "traits.hpp"

#ifndef VTM_UTILITY_MACROS
#define VTM_UTILITY_MACROS

// @SECTION VTM Utility constants

#define VTM_BIGARRAY_SIZE (10 * 1024 * 1024)

#define get_enum_mapping_1(M, I) std::get<0>(M)(I)
#define get_enum_mapping_2(M, I) std::get<1>(M)(I)
#define declare_enum_mapping(T1, T2, ...) vtm::utility::internal::enum_map<T1, T2>(__VA_ARGS__)
#define declare_enum_mapping_bool(T1, T2, ...) vtm::utility::internal::enum_map<T1, T2>(__VA_ARGS__)

#endif 

namespace vtm
{
    template<std::floating_point T, vtm::traits::StringLike S>
    inline auto string_to_float(const S& foreign_str) -> T
    {
        const std::string str{foreign_str};
        if constexpr (std::is_same_v<T, float>) return std::stof(str);
        if constexpr (std::is_same_v<T, double>) return std::stod(str);
        if constexpr (std::is_same_v<T, long double>) return std::stold(str);
    }
}

namespace vtm::utility
{
    template<vtm::traits::StringLike T = std::string>
    constexpr auto duplicate_string(T base, const int repeat, const char* seperator = " ") -> std::remove_cvref_t<T>
    {
        using string_t = std::remove_cvref_t<T>;

        string_t str;
        for (int count = 0; count < repeat; ++count) {
            str += base;
            if (count + 1 < repeat) str += seperator;
        }

        return str;
    }

    template<vtm::traits::StringLike T = std::string>
    constexpr auto duplicate_with_prefix(T prefix, T base, const int size) -> std::remove_cvref_t<T>
    {
        using string_t = std::remove_cvref_t<T>;
        string_t str = prefix;
        return prefix += duplicate_string(base, size, " ");
    }
    
    namespace internal
    {
        
        template<vtm::traits::StringLike T>
        struct oversized_array
        {
            std::array<vtm::traits::string_char_type_t<T>, VTM_BIGARRAY_SIZE> data{};
            std::size_t size;
        };

        template<vtm::traits::StringLike T>
        constexpr auto to_oversized_array(const std::remove_cvref_t<T>& str) -> oversized_array<std::remove_cvref_t<T>>
        {
            oversized_array<std::remove_cvref_t<T>> result;
            std::copy(str.begin(), str.end(), result.data.begin());
            result.size = str.size();
            return result;
        }

        template<typename Callable, vtm::traits::StringLike T>
        consteval auto to_right_sized_array(Callable callable)
        {
            constexpr auto oversized = to_oversized_array<std::remove_cvref_t<T>>(callable());
            std::array<vtm::traits::string_char_type_t<std::remove_cvref_t<T>>, oversized.size> result;
            std::copy(oversized.data.begin(), std::next(oversized.data.begin(), oversized.size), result.begin());
            return result;
        }

        template<auto Data>
        consteval const auto& make_static()
        {
            return Data;
        }

        template<typename Callable, vtm::traits::StringLike TString, vtm::traits::StringViewLike TView>
        consteval auto to_string_view(Callable callable) -> std::remove_cvref_t<TView>
        {
            using string_t = std::remove_cvref_t<TString>;
            using view_t = std::remove_cvref_t<TView>;
            constexpr auto& static_data = make_static<to_right_sized_array<decltype(callable), string_t>(callable)>();
            return view_t{static_data.begin(), static_data.end()};
        }

        template<vtm::traits::DefaultableEnum In,
                 std::semiregular Out,
                 vtm::traits::SameAsReturn<Out>... Mappings>
        inline consteval auto map_enum_values(std::invocable auto&& report_enum,
                                              std::invocable auto&& report_out,
                                              In&& default_enum,
                                              Out&& default_out,
                                              Mappings&&... mappings)
        {
            const auto options = magic_enum::enum_values<In>();
            const std::array values{ std::forward<Out>(mappings())... };

            return std::make_tuple (
                [=](const In& in) {
                    std::size_t i = 0;
                    for (const auto& o : options) {
                        if (o == in) return values[i];
                        ++i;
                    }
                    report_enum();
                    return default_out;
                },

                [=](const Out& in) {
                    std::size_t i = 0;
                    for (const auto& v : values) {
                        if (v == in) return options[i];
                        ++i;
                    }
                    report_out();
                    return default_enum;
                }
            );
        }

        template<vtm::traits::DefaultableEnum In, typename Out, typename... OutArgs>
        consteval auto enum_map(std::invocable auto report_enum,
                                std::invocable auto report_out,
                                In default_enum,
                                Out default_out,
                                OutArgs... mappings)
        {
            static_assert(sizeof...(mappings) == magic_enum::enum_count<In>() - 1, "count of enumeration mappings must be exactly the same as mapping values");
            return map_enum_values<In, Out>(std::move(report_enum),
                                            std::move(report_out),
                                            std::move(default_enum),
                                            std::move(default_out),
                                            [=]() { return Out{std::move(mappings)}; }...);
        }
    }
}

namespace vtm::utility
{
    template<std::invocable Callable,
             vtm::traits::StringLike TString = std::string,
             vtm::traits::StringViewLike TView = std::string_view>
    consteval auto compile_time_string(Callable callable) -> std::remove_cvref_t<TView>
    {
        static_assert(
            std::is_same_v<vtm::traits::string_char_type_t<TString>, vtm::traits::string_char_type_t<TView>>,
            "character types of TString and TView are not the same"
        );

        using callable_t = decltype(callable);
        return internal::to_string_view<callable_t,
                                        std::remove_cvref_t<TString>,
                                        std::remove_cvref_t<TView>   >(callable);
    }

    template<bool P, typename V>
    consteval auto static_conditional_value(V True, V False)
    {
        if constexpr (P) return True;
        else return False;
    }
}









