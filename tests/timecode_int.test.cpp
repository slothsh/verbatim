#define CATCH_CONFIG_MAIN
#include <Catch2/catch_all.hpp>
#include "catch2/catch_message.hpp"
#include "catch2/catch_test_macros.hpp"
#include "timecode.hpp"
#include <string>
#include <string_view>
#include <charconv>

TEST_CASE("vtm::timecode initialization", "[timecode][chrono][initialization]")
{
    SECTION("Default Intialization") {
        constexpr vtm::timecode tc1{};
        INFO("tc1 hours: " << tc1.hours());
        INFO("tc1 minutes: " << tc1.minutes());
        INFO("tc1 seconds: " << tc1.seconds());
        INFO("tc1 frames: " << tc1.frames());
        INFO("tc1 subframes: " << tc1.subframes());
        REQUIRE(tc1.hours() == 0);
        REQUIRE(tc1.minutes() == 0);
        REQUIRE(tc1.seconds() == 0);
        REQUIRE(tc1.frames() == 0);
        REQUIRE(tc1.subframes() == 0);
    }

    SECTION("Copy Construction") {
        vtm::timecode tc1{};
        tc1.set_hours(1);
        tc1.set_minutes(2);
        tc1.set_seconds(3);
        tc1.set_frames(4);
        tc1.set_subframes(5);

        vtm::timecode tc2{ tc1 };
        INFO("tc2 hours: " << tc2.hours());
        INFO("tc2 minutes: " << tc2.minutes());
        INFO("tc2 seconds: " << tc2.seconds());
        INFO("tc2 frames: " << tc2.frames());
        INFO("tc2 subframes: " << tc2.subframes());
        REQUIRE(tc2.hours() == 0);
        REQUIRE(tc2.minutes() == 0);
        REQUIRE(tc2.seconds() == 0);
        REQUIRE(tc2.frames() == 0);
        REQUIRE(tc2.subframes() == 0);
    }
}
