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

    for (auto& tc: tcs) {
        bool e = std::strcmp(argv[2], "true") == 0;
        const auto [ h, m, s, f, sub, fps ] = tc;
        std::cout << "timecode: " << ( (e) ? tc.enable_extended_string() : tc )
                  << " h: " << h
                  << " m: " << m
                  << " s: " << s
                  << " f: " << f
                  << " sub: " << sub
                  << " fps: " << fps
                  << '\n';
    }

    vtm::timecode tc2 = 0;
    for (std::size_t i = 0; i < 3601; ++i) {
        auto fps = vtm::fps::to_unsigned(tc2.fps());
        std::cout << "value: " << tc2 << " fps: " << fps << '\n';
        tc2 += (100 * fps);
    }

    return 0;
}
