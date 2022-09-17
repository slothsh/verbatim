#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <string_view>
#include <cstring>
#include "timecode.hpp"
#include <fmt/core.h>

int main(int argc, char** argv)
{
    std::vector<vtm::timecode> tcs { std::string_view{ "00:00:00:01" }, std::string_view{ argv[1] }, "00:00:31:00", 33055 };
    constexpr vtm::timecode tc1 { "00:00:00:01" };

    for (auto& tc: tcs) {
        bool e = std::strcmp(argv[2], "true") == 0;
        std::cout << "timecode: " << ( (e) ? tc.enable_extended_string() : tc ) << '\n';
    }

    return 0;
}
