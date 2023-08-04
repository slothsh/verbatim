// Copyright (C) Stefan Olivier
// <https://stefanolivier.com>
// ----------------------------
// Description: Type traits and interfaces

#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Standard header
#include <concepts>
#include <cstdio>
#include <iostream>
#include <stdint.h>
#include <string>
#include <type_traits>

// Library header
#include <fmt/core.h>
#include <magic_enum.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace vtm::traits {

// @SECTION CONSTRAINTS

template<typename T>
concept Regular = std::regular<T>;

template<typename T>
concept SemiRegular = std::semiregular<T>;

template<typename T, typename... Args>
concept AssignableFrom = requires (T t, Args... a) {
    { ((std::assignable_from<T&, Args>),...) };
};

template<typename T, typename... Args>
concept ConvertibleFrom = requires (T t, Args...  a) {
    { ((std::convertible_to<T, Args>),...) };
};

template<typename T>
concept FloatConvertible = ConvertibleFrom<T, float, double, long double>;

template<typename T>
concept UintConvertible = ConvertibleFrom<T, unsigned char,
                                             unsigned short,
                                             unsigned int, 
                                             unsigned long,
                                             unsigned long>;

template<typename T>
concept IntConvertible = ConvertibleFrom<T, signed char,
                                            signed short,
                                            signed int, 
                                            signed long,
                                            signed long>;

template<typename T>
concept StringConvertible = ConvertibleFrom<T, std::string, std::string_view>; // TODO: Other locale types

template<typename T, typename R, typename... Args>
concept SymmetricallyInvocable = std::invocable<T, Args...>
                              && requires (T t, Args... args) {
                                 { t(args...) } -> std::same_as<R>;
                              };

template<typename T, typename... Args>
concept SequenceInvocable = requires (T&& t, Args&&... args) {
    { ((std::invocable<T, Args>),...) };
};

template<typename T>
concept DefaultableEnum = magic_enum::is_unscoped_enum_v<T>
                       && requires (T t) {
                              T::none;
                          };

template<typename T, typename O>
concept SameAsReturn = std::same_as<std::invoke_result_t<T>, O>;

template<typename T>
concept StdStreamable = requires (T t) {
    { std::cout << t };
    { std::cerr << t };
    { std::clog << t };
};

template<typename T>
concept FmtPrintable = requires (T t) {
    { fmt::print("{}", t) };
};

template<typename T>
concept CStdIOPrintable = requires (T t) { { std::printf("%s", t)         }; }
                       || requires (T t) { { std::printf("%s", t.data())  }; }
                       || requires (T t) { { std::printf("%s", t.c_str()) }; };

template<typename T>
concept Printable = StdStreamable<T>
                 && FmtPrintable<T>
                 && CStdIOPrintable<T>;

template<typename T>
concept StringViewable = std::convertible_to<T, std::string_view>
                      && std::assignable_from<std::string_view&, T>;


// TODO: constraints for member type aliases on string interfaces
template<typename T>
concept StringLike = Regular<T>
                  && Printable<T>
                  && StringViewable<T>
                  && AssignableFrom<T, const char*, std::string, std::string_view>;

template<typename T>
concept StringViewLike = Regular<T>
                      && Printable<T>
                      && StringViewable<T>;

template<typename T>
concept StringConstructible = AssignableFrom<T, const char*, std::string, std::string_view>
                           && ConvertibleFrom<T, const char*, std::string, std::string_view>;

template<typename T>
concept StdStringInterface = std::same_as<T, std::string>
                          || std::same_as<T, std::wstring>
                          || std::same_as<T, std::u8string>
                          || std::same_as<T, std::u16string>
                          || std::same_as<T, std::u32string>
                          || std::same_as<T, std::pmr::string>
                          || std::same_as<T, std::pmr::wstring>
                          || std::same_as<T, std::pmr::u8string>
                          || std::same_as<T, std::pmr::u16string>
                          || std::same_as<T, std::pmr::u32string>
                          || std::same_as<T, std::string_view>
                          || std::same_as<T, std::wstring_view>
                          || std::same_as<T, std::u8string_view>
                          || std::same_as<T, std::u16string_view>
                          || std::same_as<T, std::u32string_view>;

} // @END OF namespace vtm::traits

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace vtm::traits {

// @SECTION Type Traits

template<typename...>
struct string_char_type_members : std::false_type
{};

template<typename T>
struct string_char_type_members<T, T> : std::true_type
{
private:
    template<StdStringInterface U>
    static constexpr auto fn(U*) -> typename U::value_type;

public:
    using type = decltype(fn<T>(nullptr));
};

template<typename T>
inline constexpr bool string_char_type_v = string_char_type_members<T, T>::value;

template<typename T>
concept StringCharType = string_char_type_v<T>;

template<typename T>
using string_char_type_t = typename string_char_type_members<T, T>::type;

template<typename...>
struct identical : std::false_type {};

template<typename T, typename... Rest>
struct identical<T, Rest...> : std::bool_constant<((std::is_same_v<T, Rest>) && ...)>
{};

template<typename... Ts>
inline constexpr bool identical_v = identical<Ts...>::value;

template<typename... Ts>
using identical_t = typename identical<Ts...>::type;

template<typename...>
struct unidentical : std::false_type {};

template<typename T, typename... Rest>
struct unidentical<T, Rest...> : std::bool_constant<((!std::is_same_v<T, Rest>) && ...)>
{};

template<typename... Ts>
using unidentical_t = typename unidentical<Ts...>::type;

template<typename... Ts>
inline constexpr bool unidentical_v = unidentical<Ts...>::value;

template<auto... Args>
struct arg_size
{
    static constexpr std::size_t value = sizeof...(Args);
};

template<auto... Args>
inline constexpr std::size_t arg_size_v = arg_size<Args...>::value;

template<std::integral T>
struct to_signed
{
    using type = std::conditional_t<std::is_signed_v<T>, T, std::make_signed_t<T>>;
};

template<std::integral T>
using to_signed_t = typename to_signed<T>::type;

template<std::integral T>
struct to_unsigned
{
    using type = std::conditional_t<std::is_unsigned_v<T>, T, std::make_unsigned_t<T>>;
};

template<std::integral T>
using to_unsigned_t = typename to_unsigned<T>::type;

template<auto Value1, auto Value2, typename T>
    requires std::is_same_v<decltype(Value1), decltype(Value2)>
struct values_equal : std::bool_constant<Value1 == Value2>
{
    using type = T;
};

template<typename T>
struct default_type : std::true_type
{
    using type = T;
};

template<int Size>
using type_of_size_t = typename std::disjunction<
        values_equal<Size, 1, std::int8_t>,
        values_equal<Size, 2, std::int16_t>,
        values_equal<Size, 4, std::int32_t>,
        values_equal<Size, 8, std::int64_t>,
        std::type_identity<void>
    >::type;

} // @END OF namespace vtm::traits

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace vtm::traits {

// @SECTION INTERFACES
// IMPORTANT
// All interfaces MUST define the member alias type 'type' to ensure
// consistency of return types across all virtual interfaces

struct __clear
{
    using type = void;
    virtual auto clear() noexcept -> type = 0;
};

struct __reset
{
    using type = void;
    virtual auto reset() noexcept -> type = 0;
};

template<StringLike T>
struct __display
{
    using type = std::remove_cvref_t<T>;
    virtual auto display() const -> type = 0;
};

template<typename T, StringLike S> // TODO: Constrain T to convertible to S
struct __display_static
{
    using type = T;
    using reference = type&;
    using display_type = S;

    auto display() const -> display_type { return display_type(static_cast<reference>(*this)); }
};

template<Regular T>
struct __implicit_type_overload
{
    using type = std::conditional_t< std::is_pointer_v<T>,
                                     T,
                                     std::remove_cvref_t<T> >;

    virtual operator type() const = 0;
};

template<Regular Arg, Regular Return>
struct __op_eq_overload
{
    using arg_type = std::remove_cvref_t<Arg>;
    using return_type = std::remove_cvref_t<Return>;
    virtual return_type operator=(const arg_type&) const = 0;
};

template<typename TString, typename TView>
struct __implicit_string_overloads : __implicit_type_overload<TString>,
                                     __implicit_type_overload<TView>
{
    using string_type = typename __implicit_type_overload<TString>::type;
    using string_view_type = typename __implicit_type_overload<TView>::type;
};

template<typename T, StringLike TString>
struct __implicit_string_overload_crtp
{
    using type = std::remove_cvref_t<T>;
    using reference = type&;
    using const_reference = std::add_const_t<type>&;
    using string_type = std::remove_cvref_t<TString>;

    operator string_type()            { return string_type(static_cast<type>(*this)); }
    operator string_type() const      { return string_type(static_cast<const_reference>(*this)); }
};

template<FloatConvertible T, std::floating_point F>
struct __convert_to_float
{
    using type = std::remove_cvref_t<T>;
    using reference = type&;
    using const_reference = std::add_const_t<type>&;
    using float_type = std::remove_cvref_t<F>;

    auto as_float() -> float_type { return float_type(static_cast<reference>(*this)); };
    auto as_float() const -> float_type { return float_type(static_cast<const_reference>(*this)); };
};

template<UintConvertible T, std::unsigned_integral U>
struct __convert_to_unsigned
{
    using type = std::remove_cvref_t<T>;
    using reference = type&;
    using const_reference = std::add_const_t<type>&;
    using unsigned_type = std::remove_cvref_t<U>;

    auto as_unsigned() -> unsigned_type { return unsigned_type(static_cast<reference>(*this)); };
    auto as_unsigned() const -> unsigned_type { return unsigned_type(static_cast<const_reference>(*this)); };
};

template<IntConvertible T, std::signed_integral I>
struct __convert_to_signed
{
    using type = std::remove_cvref_t<T>;
    using reference = type&;
    using const_reference = std::add_const_t<type>&;
    using signed_type = std::remove_cvref_t<I>;

    auto as_signed() -> signed_type { return signed_type(static_cast<reference>(*this)); };
    auto as_signed() const -> signed_type { return signed_type(static_cast<const_reference>(*this)); };
};

template<StringConvertible T, StringLike S>
struct __convert_to_string
{
    using type = std::remove_cvref_t<T>;
    using reference = type&;
    using const_reference = std::add_const_t<type>&;
    using string_type = std::remove_cvref_t<S>;

    auto as_string() -> string_type { return string_type(static_cast<reference>(*this)); };
    auto as_string() const -> string_type { return string_type(static_cast<const_reference>(*this)); };
};

template<StringConvertible T, StringViewLike S>
struct __convert_to_string_view
{
    using type = std::remove_cvref_t<T>;
    using reference = type&;
    using const_reference = std::add_const_t<type>&;
    using string_view_type = std::remove_cvref_t<S>;

    auto as_string_view() -> string_view_type { return string_view_type(static_cast<reference>(*this)); };
    auto as_string_view() const -> string_view_type { return string_view_type(static_cast<const_reference>(*this)); };
};

} // @END OF namespace vtm::traits

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace vtm::traits {

// @SECTION INTERFACE CONTRAINTS

template<typename T>
concept InterfaceClear = requires (T t) {
    { t.clear() } noexcept;
};

template<typename T>
concept InterfaceReset = requires (T t) {
    { t.reset() } noexcept;
};

template<typename T>
concept InterfaceDisplay = requires (T t) {
    { t.display() } -> std::same_as<typename T::display_t>;
};

} // @END OF namespace vtm::traits

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
