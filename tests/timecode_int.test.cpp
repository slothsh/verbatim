#define CATCH_CONFIG_MAIN
#include <Catch2/catch_all.hpp>
#include "catch2/catch_message.hpp"
#include "catch2/catch_test_macros.hpp"
#include "timecode.hpp"
#include <string>
#include <string_view>

TEST_CASE("vtm::timecode initialization", "[timecode][chrono][initialization]")
{
    SECTION("default intialization") {
        // source object
        vtm::timecode tc1{};

        // conditions for test
        INFO("tc1 hours: "     << tc1.hours());     CHECK(tc1.hours()     == 0);
        INFO("tc1 minutes: "   << tc1.minutes());   CHECK(tc1.minutes()   == 0);
        INFO("tc1 seconds: "   << tc1.seconds());   CHECK(tc1.seconds()   == 0);
        INFO("tc1 frames: "    << tc1.frames());    CHECK(tc1.frames()    == 0);
        INFO("tc1 subframes: " << tc1.subframes()); CHECK(tc1.subframes() == 0);
        INFO("tc1 fps: "       << tc1.fps());       CHECK(tc1.fps()       == vtm::fps::default_value());

        // TODO: check flags
    }

    SECTION("copy construction") {
        // source object
        vtm::timecode tc1{};
        tc1.set_hours(42);
        tc1.set_minutes(42);
        tc1.set_seconds(42);
        tc1.set_frames(23);
        tc1.set_subframes(42);
        tc1.set_fps(vtm::fps::fps_24);

        // pre-conditions for valid copy construction
        INFO("tc1 hours: "     << tc1.hours());     REQUIRE(tc1.hours()     == 42);
        INFO("tc1 minutes: "   << tc1.minutes());   REQUIRE(tc1.minutes()   == 42);
        INFO("tc1 seconds: "   << tc1.seconds());   REQUIRE(tc1.seconds()   == 42);
        INFO("tc1 frames: "    << tc1.frames());    REQUIRE(tc1.frames()    == 23);
        INFO("tc1 subframes: " << tc1.subframes()); REQUIRE(tc1.subframes() == 42);
        INFO("tc1 fps: "       << tc1.fps());       REQUIRE(tc1.fps()       == vtm::fps::fps_24);

        // destination object
        vtm::timecode tc2{ tc1 };

        // condtions for test
        INFO("tc2 hours: "     << tc2.hours());     CHECK(tc2.hours()     == 42);
        INFO("tc2 minutes: "   << tc2.minutes());   CHECK(tc2.minutes()   == 42);
        INFO("tc2 seconds: "   << tc2.seconds());   CHECK(tc2.seconds()   == 42);
        INFO("tc2 frames: "    << tc2.frames());    CHECK(tc2.frames()    == 23);
        INFO("tc2 subframes: " << tc2.subframes()); CHECK(tc2.subframes() == 42);
        INFO("tc2 fps: "       << tc2.fps());       CHECK(tc2.fps()       == vtm::fps::fps_24);

        // post-conditions for copy construction
        INFO("tc1 hours: "     << tc1.hours());     CHECK(tc1.hours()     == 42);
        INFO("tc1 minutes: "   << tc1.minutes());   CHECK(tc1.minutes()   == 42);
        INFO("tc1 seconds: "   << tc1.seconds());   CHECK(tc1.seconds()   == 42);
        INFO("tc1 frames: "    << tc1.frames());    CHECK(tc1.frames()    == 23);
        INFO("tc1 subframes: " << tc1.subframes()); CHECK(tc1.subframes() == 42);
        INFO("tc1 fps: "       << tc1.fps());       CHECK(tc1.fps()       == vtm::fps::fps_24);

        // TODO: check flags
    }

    SECTION("move construction") {
        // source object
        vtm::timecode tc1{};
        tc1.set_hours(42);
        tc1.set_minutes(42);
        tc1.set_seconds(42);
        tc1.set_frames(23);
        tc1.set_subframes(42);
        tc1.set_fps(vtm::fps::fps_24);

        // pre-conditions for valid copy construction
        INFO("tc1 hours: "     << tc1.hours());     REQUIRE(tc1.hours()     == 42);
        INFO("tc1 minutes: "   << tc1.minutes());   REQUIRE(tc1.minutes()   == 42);
        INFO("tc1 seconds: "   << tc1.seconds());   REQUIRE(tc1.seconds()   == 42);
        INFO("tc1 frames: "    << tc1.frames());    REQUIRE(tc1.frames()    == 23);
        INFO("tc1 subframes: " << tc1.subframes()); REQUIRE(tc1.subframes() == 42);
        INFO("tc1 fps: "       << tc1.fps());       REQUIRE(tc1.fps()       == vtm::fps::fps_24);

        // destination object
        vtm::timecode tc2{ std::move(tc1) };

        // condtions for test
        INFO("tc2 hours: "     << tc2.hours());     CHECK(tc2.hours()     == 42);
        INFO("tc2 minutes: "   << tc2.minutes());   CHECK(tc2.minutes()   == 42);
        INFO("tc2 seconds: "   << tc2.seconds());   CHECK(tc2.seconds()   == 42);
        INFO("tc2 frames: "    << tc2.frames());    CHECK(tc2.frames()    == 23);
        INFO("tc2 subframes: " << tc2.subframes()); CHECK(tc2.subframes() == 42);
        INFO("tc2 fps: "       << tc2.fps());       CHECK(tc2.fps()       == vtm::fps::fps_24);

        // post-conditions for copy construction
        INFO("tc1 hours: "     << tc1.hours());     CHECK(tc1.hours()     == 0);
        INFO("tc1 minutes: "   << tc1.minutes());   CHECK(tc1.minutes()   == 0);
        INFO("tc1 seconds: "   << tc1.seconds());   CHECK(tc1.seconds()   == 0);
        INFO("tc1 frames: "    << tc1.frames());    CHECK(tc1.frames()    == 0);
        INFO("tc1 subframes: " << tc1.subframes()); CHECK(tc1.subframes() == 0);
        INFO("tc1 fps: "       << tc1.fps());       CHECK(tc1.fps()       == vtm::fps::default_value());

        // TODO: check flags
    }
}

TEST_CASE("vtm::timecode set value semantics", "[chrono][timecode][conversion][number]")
{
    SECTION("groups can be set independently") {
        // default object
        vtm::timecode tc1{};
        tc1.set_hours(42);
        tc1.set_minutes(42);
        tc1.set_seconds(42);
        tc1.set_frames(12);
        tc1.set_subframes(69);
        tc1.set_fps(vtm::fps::fps_25);

        // conditions for test
        INFO("tc1 hours: "     << tc1.hours());     REQUIRE(tc1.hours()     == 42);
        INFO("tc1 minutes: "   << tc1.minutes());   REQUIRE(tc1.minutes()   == 42);
        INFO("tc1 seconds: "   << tc1.seconds());   REQUIRE(tc1.seconds()   == 42);
        INFO("tc1 frames: "    << tc1.frames());    REQUIRE(tc1.frames()    == 12);
        INFO("tc1 subframes: " << tc1.subframes()); REQUIRE(tc1.subframes() == 69);
        INFO("tc1 fps: "       << tc1.fps());       REQUIRE(tc1.fps()       == vtm::fps::fps_25);
    }

    SECTION("groups will cascade with correct group values") {
        // default object
        vtm::timecode tc1{};
        tc1.set_hours(4);                // 4 hours
        tc1.set_minutes(2 * 60 + 42);    // 2 hours   , 42 minutes
        tc1.set_seconds(6 * 60 + 31);    // 6 minutes , 31 seconds
        tc1.set_frames(9 * 25 + 12);     // 9 seconds , 12 frames
        tc1.set_subframes(4 * 100 + 69); // 4 frames  , 69 subframes
        tc1.set_fps(vtm::fps::fps_25);

        // conditions for test
        INFO("tc1 hours: "     << tc1.hours());     CHECK(tc1.hours()     == 2);
        INFO("tc1 minutes: "   << tc1.minutes());   CHECK(tc1.minutes()   == 6);
        INFO("tc1 seconds: "   << tc1.seconds());   CHECK(tc1.seconds()   == 9);
        INFO("tc1 frames: "    << tc1.frames());    CHECK(tc1.frames()    == 4);
        INFO("tc1 subframes: " << tc1.subframes()); CHECK(tc1.subframes() == 69);
        INFO("tc1 fps: "       << tc1.fps());       CHECK(tc1.fps()       == vtm::fps::fps_25);
    }

    SECTION("groups won't be erased when cascading ticks") {
        // default object
        vtm::timecode tc1{};
        tc1.set_hours(4);                // 4 hours
        tc1.set_minutes(31);             // 31 minutes
        tc1.set_seconds(42);             // 42 seconds
        tc1.set_frames(12);              // 12 frames
        tc1.set_subframes(4 * 100 + 69); // 4 frames, 69 subframes
        tc1.set_fps(vtm::fps::fps_25);

        // conditions for test
        INFO("tc1 hours: "     << tc1.hours());     CHECK(tc1.hours()     == 4);
        INFO("tc1 minutes: "   << tc1.minutes());   CHECK(tc1.minutes()   == 31);
        INFO("tc1 seconds: "   << tc1.seconds());   CHECK(tc1.seconds()   == 42);
        INFO("tc1 frames: "    << tc1.frames());    CHECK(tc1.frames()    == 4);
        INFO("tc1 subframes: " << tc1.subframes()); CHECK(tc1.subframes() == 69);
        INFO("tc1 fps: "       << tc1.fps());       CHECK(tc1.fps()       == vtm::fps::default_value());
    }
}

TEST_CASE("vtm::timecode tick value representation", "[chrono][timecode][number]")
{
    SECTION("setting ticks is calculated as expected") {
        // default object
        vtm::timecode tc1{};

        tc1.set_ticks(
            1 * 60 * 60 * 25 * 100 +
            2 * 60 * 25 * 100 +
            3 * 25 * 100 +
            4 * 100 +
            5
        );

        // pre-conditions for test
        INFO("tc1 fps: "   << tc1.fps());   REQUIRE(tc1.fps() == vtm::fps::default_value());

        // conditions for test
        INFO("tc1 hours: "     << tc1.hours());     CHECK(tc1.hours()     == 1);
        INFO("tc1 minutes: "   << tc1.minutes());   CHECK(tc1.minutes()   == 2);
        INFO("tc1 seconds: "   << tc1.seconds());   CHECK(tc1.seconds()   == 3);
        INFO("tc1 frames: "    << tc1.frames());    CHECK(tc1.frames()    == 4);
        INFO("tc1 subframes: " << tc1.subframes()); CHECK(tc1.subframes() == 5);
    }

    SECTION("setting ticks by group is calculated as expected") {
        // default object
        vtm::timecode tc1{};

        tc1.set_hours(1);
        tc1.set_minutes(2);
        tc1.set_seconds(3);
        tc1.set_frames(4);
        tc1.set_subframes(5);

        // pre-conditions for test
        INFO("tc1 fps: "   << tc1.fps());   REQUIRE(tc1.fps() == vtm::fps::default_value());

        // conditions for test
        INFO("tc1 ticks: " << tc1.ticks()); CHECK(tc1.ticks() == (1 * 60 * 60 * 25 * 100) +
                                                                 (2 * 60 * 25 * 100) +
                                                                 (3 * 25 * 100) +
                                                                 (4 * 100) +
                                                                 5);

    }
}

TEST_CASE("vtm::timecode signed type conversions", "[chrono][timecode][conversion][number]")
{
    SECTION("signed type conversion returns correct ticks for each TC group") {
        // source objects
        vtm::timecode tc1{};

        // destination object
        vtm::timecode::signed_type _int1 = 0;

        // pre-conditions for test
        INFO("tc1 hours: "     << tc1.hours());     REQUIRE(tc1.hours()     == 0);
        INFO("tc1 minutes: "   << tc1.minutes());   REQUIRE(tc1.minutes()   == 0);
        INFO("tc1 seconds: "   << tc1.seconds());   REQUIRE(tc1.seconds()   == 0);
        INFO("tc1 frames: "    << tc1.frames());    REQUIRE(tc1.frames()    == 0);
        INFO("tc1 subframes: " << tc1.subframes()); REQUIRE(tc1.subframes() == 0);
        INFO("tc1 fps: "       << tc1.fps());       REQUIRE(tc1.fps()       == vtm::fps::default_value());

        // check hours group
        tc1.reset(); tc1.set_hours(1);
        _int1 = tc1; CHECK(_int1 == 1 * 60 * 60 * 25 * 100);
        INFO("tc1 hours: "     << tc1.hours());
        INFO("tc1 minutes: "   << tc1.minutes());
        INFO("tc1 seconds: "   << tc1.seconds());
        INFO("tc1 frames: "    << tc1.frames());
        INFO("tc1 subframes: " << tc1.subframes());

        // check mintues group
        tc1.reset(); tc1.set_minutes(1);
        _int1 = tc1; CHECK(_int1 == 1 * 60 * 25 * 100);
        INFO("tc1 hours: "     << tc1.hours());
        INFO("tc1 minutes: "   << tc1.minutes());
        INFO("tc1 seconds: "   << tc1.seconds());
        INFO("tc1 frames: "    << tc1.frames());
        INFO("tc1 subframes: " << tc1.subframes());

        // check seconds group
        tc1.reset(); tc1.set_seconds(1);
        _int1 = tc1; CHECK(_int1 == 1 * 25 * 100);
        INFO("tc1 hours: "     << tc1.hours());
        INFO("tc1 minutes: "   << tc1.minutes());
        INFO("tc1 seconds: "   << tc1.seconds());
        INFO("tc1 frames: "    << tc1.frames());
        INFO("tc1 subframes: " << tc1.subframes());

        // check frames group
        tc1.reset(); tc1.set_frames(25);
        _int1 = tc1; CHECK(_int1 == 1 * 25 * 100);
        INFO("tc1 hours: "     << tc1.hours());
        INFO("tc1 minutes: "   << tc1.minutes());
        INFO("tc1 seconds: "   << tc1.seconds());
        INFO("tc1 frames: "    << tc1.frames());
        INFO("tc1 subframes: " << tc1.subframes());

        // check subframes group
        tc1.reset(); tc1.set_subframes(100);
        _int1 = tc1; CHECK(_int1 == 100);
        INFO("tc1 hours: "     << tc1.hours());
        INFO("tc1 minutes: "   << tc1.minutes());
        INFO("tc1 seconds: "   << tc1.seconds());
        INFO("tc1 frames: "    << tc1.frames());
        INFO("tc1 subframes: " << tc1.subframes());
    }
}

TEST_CASE("vtm::timecode string conversions", "[chrono][timecode][conversion][string]")
{
    SECTION("construct and convert to string type") {
        vtm::timecode t1{};
        std::string str1 = t1;
        INFO("tc1 string: " << str1);
        REQUIRE(str1 == "00:00:00:00:00");
        t1.set_hours(59);
        std::string str2 = t1;
        INFO("tc2 string: " << str2);
        REQUIRE(str2 == "59:00:00:00:00");
    }
}
