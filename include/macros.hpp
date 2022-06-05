// Stefan Olivier
// Description: Various helper macros

#pragma once

// Standard library
#include <array>
#include <cstdint>
#include <string_view>

// Third-party library
#include <magic_enum.hpp>

// Project library
#include "traits.hpp"
#include "errors.hpp"

// Macro utility functions
#ifndef VTM_MACROS
#define VTM_MACROS

// TODO: Template on string type

#define lambda(R) []() noexcept { return R; }
#define lambda_sv(R) []() noexcept { return std::string_view{ R }; }

#endif

















