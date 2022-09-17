#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "timecode.hpp"
#include <vector>
#include <string>
#include <string_view>

TEST_CASE("type conversions for chrono fps", "[timecode][chrono][framerate][conversion][static]")
{
    using namespace vtm::chrono;
    const auto fps_24   = fps::fps_24;
    const auto fps_25   = fps::fps_25;
    const auto fps_29   = fps::fps_29p97;
    const auto fps_29df = fps::fpsdf_29p97;
    const auto fps_30   = fps::fps_30;
    const auto fps_60   = fps::fps_60;

    vtm::fpsfloat_t fps_float_24   = fps::to_float(fps_24);   vtm::fpsint_t fps_int_24   = fps::to_signed(fps_24);   auto fps_str_24   = fps::to_string(fps_24);
    vtm::fpsfloat_t fps_float_25   = fps::to_float(fps_25);   vtm::fpsint_t fps_int_25   = fps::to_signed(fps_25);   auto fps_str_25   = fps::to_string(fps_25);
    vtm::fpsfloat_t fps_float_29   = fps::to_float(fps_29);   vtm::fpsint_t fps_int_29   = fps::to_signed(fps_29);   auto fps_str_29   = fps::to_string(fps_29);
    vtm::fpsfloat_t fps_float_29df = fps::to_float(fps_29df); vtm::fpsint_t fps_int_29df = fps::to_signed(fps_29df); auto fps_str_29df = fps::to_string(fps_29df);
    vtm::fpsfloat_t fps_float_30   = fps::to_float(fps_30);   vtm::fpsint_t fps_int_30   = fps::to_signed(fps_30);   auto fps_str_30   = fps::to_string(fps_30);
    vtm::fpsfloat_t fps_float_60   = fps::to_float(fps_60);   vtm::fpsint_t fps_int_60   = fps::to_signed(fps_60);   auto fps_str_60   = fps::to_string(fps_60);

    INFO("fps_float_24: " <<   fps_float_24 << "\tfps_int_24: " <<     fps_int_24 << "\t\tfps_str_24: " <<   fps_str_24);
    INFO("fps_float_25: " <<   fps_float_25 << "\tfps_int_25: " <<     fps_int_25 << "\t\tfps_str_25: " <<   fps_str_25);
    INFO("fps_float_29: " <<   fps_float_29 << "\tfps_int_29: " <<     fps_int_29 << "\t\tfps_str_29: " <<   fps_str_29);
    INFO("fps_float_29df: " << fps_float_29df << "\tfps_int_29df: " << fps_int_29df << "\tfps_str_29df: " << fps_str_29df);
    INFO("fps_float_30: " <<   fps_float_30 << "\tfps_int_30: " <<     fps_int_30 << "\t\tfps_str_30: " <<   fps_str_30);
    INFO("fps_float_60: " <<   fps_float_60 << "\tfps_int_60: " <<     fps_int_60 << "\t\tfps_str_60: " <<   fps_str_60);

    REQUIRE(fps_float_24 == 24.0);    REQUIRE(fps_int_24 == 24);   REQUIRE(fps_str_24 == "24 fps");
    REQUIRE(fps_float_25 == 25.0);    REQUIRE(fps_int_25 == 25);   REQUIRE(fps_str_25 == "25 fps");
    REQUIRE(fps_float_29 == 29.97);   REQUIRE(fps_int_29 == 29);   REQUIRE(fps_str_29 == "29.97 fps");
    REQUIRE(fps_float_29df == 29.97); REQUIRE(fps_int_29df == 29); REQUIRE(fps_str_29df == "29.97 fps drop-frame");
    REQUIRE(fps_float_30 == 30.0);    REQUIRE(fps_int_30 == 30);   REQUIRE(fps_str_30 == "30 fps");
    REQUIRE(fps_float_60 == 60.0);    REQUIRE(fps_int_60 == 60);   REQUIRE(fps_str_60 == "60 fps");
}

