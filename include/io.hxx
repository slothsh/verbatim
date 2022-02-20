// Stefan Olivier
// Description: IO definitions and coniguration

#pragma once

#include <fstream>

namespace sostd::locale
{
    using STRING_TYPE      = std::string;
    using STRING_VIEW_TYPE = std::string_view;
}

namespace sostd::io
{
    constexpr auto FSTREAM_IN      = std::ios::in  | std::ios::binary;
    constexpr auto FSTREAM_IN_ATE  = std::ios::in  | std::ios::ate | std::ios::binary;
    constexpr auto FSTREAM_OUT     = std::ios::out | std::ios::binary;
    constexpr auto FSTREAM_OUT_ATE = std::ios::out | std::ios::ate | std::ios::binary;
}
