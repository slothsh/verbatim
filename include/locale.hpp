// Stefan Olivier
// Description: Locale defintions and configuration

#pragma once

#include <string>
#include <string_view>

namespace vtm::locale
{
    using STRING_TYPE      = std::string;
    using STRING_VIEW_TYPE = std::string_view;
}

namespace vtm
{
    using string      = vtm::locale::STRING_TYPE;
    using string_view = vtm::locale::STRING_VIEW_TYPE;
}
