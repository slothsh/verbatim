// Stefan Olivier
// Description: Functional procedures and types

#pragma once

// Standard headers
#include <concepts>
#include <type_traits>

// Project headers
#include "traits.hpp"

namespace vtm::fn
{
    template<typename C, typename... Ts>
        requires vtm::traits::SequenceInvocable<C, Ts...>
    constexpr auto zip_results(C&& fn, Ts&&... args)
    {
        return std::make_tuple(std::forward<C>(fn(std::forward<Ts>(args)))...);
    }
}


