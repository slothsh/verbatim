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
    SECTION("Default intialization") {
        constexpr vtm::timecode tc1{};
        INFO("tc1 hours: " << tc1.hours());
        INFO("tc1 minutes: " << tc1.minutes());
        INFO("tc1 seconds: " << tc1.seconds());
        INFO("tc1 frames: " << tc1.frames());
        INFO("tc1 subframes: " << tc1.subframes());

        CHECK(tc1.hours() == 0);
        CHECK(tc1.minutes() == 0);
        CHECK(tc1.seconds() == 0);
        CHECK(tc1.frames() == 0);
        CHECK(tc1.subframes() == 0);
    }

    SECTION("Copy Construction") {
        vtm::timecode tc1{};
        tc1.set_fps(vtm::fps::fps_25);

        tc1.set_hours(42);
        {
            vtm::timecode tc2{ tc1 };
            INFO("tc2 hours: " << tc2.hours());
            INFO("tc2 minutes: " << tc2.minutes());
            INFO("tc2 seconds: " << tc2.seconds());
            INFO("tc2 frames: " << tc2.frames());
            INFO("tc2 subframes: " << tc2.subframes());

            CHECK(tc2.hours() == 42);
            CHECK(tc2.minutes() == 0);
            CHECK(tc2.seconds() == 0);
            CHECK(tc2.frames() == 0);
            CHECK(tc2.subframes() == 0);
        }

        tc1.set_minutes(42);
        {
            vtm::timecode tc2{ tc1 };
            INFO("tc2 hours: " << tc2.hours());
            INFO("tc2 minutes: " << tc2.minutes());
            INFO("tc2 seconds: " << tc2.seconds());
            INFO("tc2 frames: " << tc2.frames());
            INFO("tc2 subframes: " << tc2.subframes());

            CHECK(tc2.hours() == 0);
            CHECK(tc2.minutes() == 42);
            CHECK(tc2.seconds() == 0);
            CHECK(tc2.frames() == 0);
            CHECK(tc2.subframes() == 0);
        }

        tc1.set_seconds(42);
        {
            vtm::timecode tc2{ tc1 };
            INFO("tc2 hours: " << tc2.hours());
            INFO("tc2 minutes: " << tc2.minutes());
            INFO("tc2 seconds: " << tc2.seconds());
            INFO("tc2 frames: " << tc2.frames());
            INFO("tc2 subframes: " << tc2.subframes());

            CHECK(tc2.hours() == 0);
            CHECK(tc2.minutes() == 0);
            CHECK(tc2.seconds() == 42);
            CHECK(tc2.frames() == 0);
            CHECK(tc2.subframes() == 0);
        }

        tc1.set_frames(24);
        {
            vtm::timecode tc2{ tc1 };
            INFO("tc2 hours: " << tc2.hours());
            INFO("tc2 minutes: " << tc2.minutes());
            INFO("tc2 seconds: " << tc2.seconds());
            INFO("tc2 frames: " << tc2.frames());
            INFO("tc2 subframes: " << tc2.subframes());

            CHECK(tc2.hours() == 0);
            CHECK(tc2.minutes() == 0);
            CHECK(tc2.seconds() == 0);
            CHECK(tc2.frames() == 24);
            CHECK(tc2.subframes() == 0);
        }

        tc1.set_subframes(42);
        {
            vtm::timecode tc2{ tc1 };
            INFO("tc2 hours: " << tc2.hours());
            INFO("tc2 minutes: " << tc2.minutes());
            INFO("tc2 seconds: " << tc2.seconds());
            INFO("tc2 frames: " << tc2.frames());
            INFO("tc2 subframes: " << tc2.subframes());

            CHECK(tc2.hours() == 0);
            CHECK(tc2.minutes() == 0);
            CHECK(tc2.seconds() == 0);
            CHECK(tc2.frames() == 0);
            CHECK(tc2.subframes() == 42);
        }
    }
}

TEST_CASE("vtm::timecode signed type conversions", "[chrono][timecode][conversion][number]")
{
    SECTION("construct and convert to signed types")
    {
        vtm::timecode tc1{};
        vtm::timecode::signed_type _int1 = 0;
        tc1.set_fps(vtm::fps::fps_25);

        tc1.set_hours(1);
        INFO("tc1 hours: " << tc1.hours()); INFO("tc1 minutes: " << tc1.minutes()); INFO("tc1 seconds: " << tc1.seconds());
        INFO("tc1 frames: " << tc1.frames()); INFO("tc1 subframes: " << tc1.subframes());
        _int1 = tc1;
        CHECK(_int1 == 1 * 60 * 60 * 25 * 100);

        tc1.set_hours(0);
        tc1.set_minutes(1);
        INFO("tc1 hours: " << tc1.hours()); INFO("tc1 minutes: " << tc1.minutes()); INFO("tc1 seconds: " << tc1.seconds());
        INFO("tc1 frames: " << tc1.frames()); INFO("tc1 subframes: " << tc1.subframes());
        _int1 = tc1;
        CHECK(_int1 == 1 * 60 * 25 * 100);

        tc1.set_minutes(0);
        tc1.set_seconds(1);
        INFO("tc1 hours: " << tc1.hours()); INFO("tc1 minutes: " << tc1.minutes()); INFO("tc1 seconds: " << tc1.seconds());
        INFO("tc1 frames: " << tc1.frames()); INFO("tc1 subframes: " << tc1.subframes());
        _int1 = tc1;
        CHECK(_int1 == 1 * 25 * 100);

        tc1.set_seconds(0);
        tc1.set_frames(25);
        INFO("tc1 hours: " << tc1.hours()); INFO("tc1 minutes: " << tc1.minutes()); INFO("tc1 seconds: " << tc1.seconds());
        INFO("tc1 frames: " << tc1.frames()); INFO("tc1 subframes: " << tc1.subframes());
        _int1 = tc1;
        CHECK(_int1 == 1 * 25 * 100);

        tc1.set_frames(0);
        tc1.set_subframes(100);
        INFO("tc1 hours: " << tc1.hours()); INFO("tc1 minutes: " << tc1.minutes()); INFO("tc1 seconds: " << tc1.seconds());
        INFO("tc1 frames: " << tc1.frames()); INFO("tc1 subframes: " << tc1.subframes());
        _int1 = tc1;
        CHECK(_int1 == 100);
    }
}

TEST_CASE("vtm::timecode string conversions", "[chrono][timecode][conversion][string]")
{
    SECTION("construct and convert to string type")
    {
        constexpr vtm::timecode t1{};
        std::string str1 = t1;
        INFO("tc1 string: " << str1);
        REQUIRE(str1 == "00:00:00:00:00");
    }
}
