#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "timecode.hpp"
#include <string>
#include <string_view>

///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION Static Tests --
//
///////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_STATIC_TESTS
#define TOTAL_SCALAR_GROUPS 5

// static helpers
consteval int to_nearest_padding(int size, int alignment)
{
    int boundary = 0;
    while (boundary < size) {
        boundary += alignment;
    }

    return boundary - size;
}

consteval int size_of_vtmtimecode()
{
#if defined(_WIN32)
    constexpr int size = (sizeof(vtm::timecode::scalar_t) * TOTAL_SCALAR_GROUPS) +
                          sizeof(vtm::timecode::fps_scalar_t) +
                          sizeof(vtm::timecode::flags_t) +
                          sizeof(void*);
#elif defined(__linux__) || defined(__gnu_linux__)
    constexpr int size = (sizeof(vtm::timecode::scalar_t) * TOTAL_SCALAR_GROUPS) +
                          sizeof(vtm::timecode::fps_scalar_t) +
                          sizeof(vtm::timecode::flags_t);
#endif

    constexpr int padding = to_nearest_padding(size, alignof(vtm::timecode));

    return size + padding;
}

// conditions for static tests
static_assert(sizeof(vtm::timecode) == size_of_vtmtimecode(), "size of vtm::timecode object does not meet established size requirements");

#undef TOTAL_SCALAR_GROUPS
#endif // @END OF ENABLE_STATIC_TESTS

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION Initialization Tests --
//
///////////////////////////////////////////////////////////////////////////

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

    SECTION("integral subsumption initialization"){
        // source object
        vtm::timecode tc1{ (1 * 60 * 60 * 25 * 100) +
                           (2 * 60 * 25 * 100) +
                           (3 * 25 * 100) +
                           (4 * 100) +
                            5
                         };

        // pre-conditions for valid copy construction
        INFO("tc1 fps: "       << tc1.fps());       REQUIRE(tc1.fps()       == vtm::fps::default_value());

        // condtions for test
        INFO("tc1 hours: "     << tc1.hours());     CHECK(tc1.hours()     == 1);
        INFO("tc1 minutes: "   << tc1.minutes());   CHECK(tc1.minutes()   == 2);
        INFO("tc1 seconds: "   << tc1.seconds());   CHECK(tc1.seconds()   == 3);
        INFO("tc1 frames: "    << tc1.frames());    CHECK(tc1.frames()    == 4);
        INFO("tc1 subframes: " << tc1.subframes()); CHECK(tc1.subframes() == 5);
    }

    SECTION("string view subsumption initialization with extended timecode form"){
        // source object
        vtm::timecode tc1{ "00:00:00:00.00" };
        vtm::timecode tc2{ "12:34:56:12.34" };

        // pre-conditions for valid copy construction
        INFO("tc1 fps: "       << tc1.fps());       REQUIRE(tc1.fps()       == vtm::fps::default_value());
        INFO("tc2 fps: "       << tc2.fps());       REQUIRE(tc2.fps()       == vtm::fps::default_value());

        // condtions for test
        INFO("tc1 ticks: "     << tc1.ticks());     CHECK(tc1.ticks()     == 0);
        INFO("tc1 hours: "     << tc1.hours());     CHECK(tc1.hours()     == 0);
        INFO("tc1 minutes: "   << tc1.minutes());   CHECK(tc1.minutes()   == 0);
        INFO("tc1 seconds: "   << tc1.seconds());   CHECK(tc1.seconds()   == 0);
        INFO("tc1 frames: "    << tc1.frames());    CHECK(tc1.frames()    == 0);
        INFO("tc1 subframes: " << tc1.subframes()); CHECK(tc1.subframes() == 0);

        INFO("tc2 ticks: "     << tc2.ticks());     CHECK(tc2.ticks()     == (12 * 60 * 60 * 25 * 100) +
                                                                             (34 * 60 * 25 * 100) +
                                                                             (56 * 25 * 100) +
                                                                             (12 * 100) +
                                                                              34);

        INFO("tc2 hours: "     << tc2.hours());     CHECK(tc2.hours()     == 12);
        INFO("tc2 minutes: "   << tc2.minutes());   CHECK(tc2.minutes()   == 34);
        INFO("tc2 seconds: "   << tc2.seconds());   CHECK(tc2.seconds()   == 56);
        INFO("tc2 frames: "    << tc2.frames());    CHECK(tc2.frames()    == 12);
        INFO("tc2 subframes: " << tc2.subframes()); CHECK(tc2.subframes() == 34);
    }

    SECTION("string view subsumption initialization with standard timecode form"){
        // source object
        vtm::timecode tc1{ "00:00:00:00" };
        vtm::timecode tc2{ "12:34:56:12" };

        // pre-conditions for valid copy construction
        INFO("tc1 fps: "       << tc1.fps());       REQUIRE(tc1.fps()       == vtm::fps::default_value());
        INFO("tc2 fps: "       << tc2.fps());       REQUIRE(tc2.fps()       == vtm::fps::default_value());

        // condtions for test
        INFO("tc1 ticks: "     << tc1.ticks());     CHECK(tc1.ticks()     == 0);
        INFO("tc1 hours: "     << tc1.hours());     CHECK(tc1.hours()     == 0);
        INFO("tc1 minutes: "   << tc1.minutes());   CHECK(tc1.minutes()   == 0);
        INFO("tc1 seconds: "   << tc1.seconds());   CHECK(tc1.seconds()   == 0);
        INFO("tc1 frames: "    << tc1.frames());    CHECK(tc1.frames()    == 0);
        INFO("tc1 subframes: " << tc1.subframes()); CHECK(tc1.subframes() == 0);

        INFO("tc2 ticks: "     << tc2.ticks());     CHECK(tc2.ticks()     == (12 * 60 * 60 * 25 * 100) +
                                                                             (34 * 60 * 25 * 100) +
                                                                             (56 * 25 * 100) +
                                                                             (12 * 100));

        INFO("tc2 hours: "     << tc2.hours());     CHECK(tc2.hours()     == 12);
        INFO("tc2 minutes: "   << tc2.minutes());   CHECK(tc2.minutes()   == 34);
        INFO("tc2 seconds: "   << tc2.seconds());   CHECK(tc2.seconds()   == 56);
        INFO("tc2 frames: "    << tc2.frames());    CHECK(tc2.frames()    == 12);
        INFO("tc2 subframes: " << tc2.subframes()); CHECK(tc2.subframes() == 0);
    }
}

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION Value Modification Semantics Tests --
//
///////////////////////////////////////////////////////////////////////////

TEST_CASE("vtm::timecode value modification semantics", "[chrono][timecode][conversion][number]")
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

    SECTION("groups will cascade from top to bottom with correct values") {
        // default object
        vtm::timecode tc1{};

        // pre-conditions for test
        INFO("tc1 fps: " << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());


        // conditions for test
        tc1.set_hours(4);                // 4 hours
        INFO("tc1 hours: " << tc1.hours()); CHECK(tc1.hours() == 4);

        tc1.set_minutes(2 * 60 + 42);    // 2 hours   , 42 minutes
        INFO("tc1 hours: "   << tc1.hours());   CHECK(tc1.hours()   == 2);
        INFO("tc1 minutes: " << tc1.minutes()); CHECK(tc1.minutes() == 42);

        tc1.set_seconds(6 * 60 + 31);    // 6 minutes , 31 seconds
        INFO("tc1 minutes: " << tc1.minutes()); CHECK(tc1.minutes() == 6);
        INFO("tc1 seconds: " << tc1.seconds()); CHECK(tc1.seconds() == 31);

        tc1.set_frames(9 * 25 + 12);     // 9 seconds , 12 frames
        INFO("tc1 seconds: " << tc1.seconds()); CHECK(tc1.seconds() == 9);
        INFO("tc1 frames: "  << tc1.frames());  CHECK(tc1.frames()  == 12);

        tc1.set_subframes(4 * 100 + 69); // 4 frames  , 69 subframes
        INFO("tc1 frames: "    << tc1.frames());    CHECK(tc1.frames()    == 4);
        INFO("tc1 subframes: " << tc1.subframes()); CHECK(tc1.subframes() == 69);
    }

    SECTION("groups won't be erased when cascading ticks") {
        // default object
        vtm::timecode tc1{};

        // pre-conditions for test
        INFO("tc1 fps: "       << tc1.fps());       REQUIRE(tc1.fps()     == vtm::fps::default_value());

        tc1.set_hours(4);                // 4 hours
        tc1.set_minutes(31);             // 31 minutes
        tc1.set_seconds(42);             // 42 seconds
        tc1.set_frames(12);              // 12 frames
        tc1.set_subframes(4 * 100 + 69); // 4 frames, 69 subframes

        // conditions for test
        INFO("tc1 hours: "     << tc1.hours());     CHECK(tc1.hours()     == 4);
        INFO("tc1 minutes: "   << tc1.minutes());   CHECK(tc1.minutes()   == 31);
        INFO("tc1 seconds: "   << tc1.seconds());   CHECK(tc1.seconds()   == 42);
        INFO("tc1 frames: "    << tc1.frames());    CHECK(tc1.frames()    == 4);
        INFO("tc1 subframes: " << tc1.subframes()); CHECK(tc1.subframes() == 69);
    }
}

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION Tick Value Representation Tests --
//
///////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION Integral Assignment Tests --
//
///////////////////////////////////////////////////////////////////////////

TEST_CASE("vtm::timecode integral assignment", "[chrono][timecode][conversion][number]")
{
    SECTION("assignment to signed types return correct tick values") {
         // default objects
        vtm::timecode tc1{};
        vtm::timecode tc2{};
        vtm::timecode tc3{};
        vtm::timecode tc4{};

        tc1 = (12 * 60 * 60 * 25 * 100) + (34 * 60 * 25 * 100) + (56 * 25 * 100) + (12 * 100) + (34);
        tc2 = (12 * 60 * 60 * 25 * 100) + (34 * 60 * 25 * 100) + (56 * 25 * 100) + (12 * 100) + (34);
        tc3 = (12 * 60 * 60 * 25 * 100) + (34 * 60 * 25 * 100) + (56 * 25 * 100) + (12 * 100) + (34);
        tc4 = (12 * 60 * 60 * 25 * 100) + (34 * 60 * 25 * 100) + (56 * 25 * 100) + (12 * 100) + (34);

        // pre-conditions for test
        INFO("tc1 fps: "    << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());
        INFO("tc2 fps: "    << tc2.fps()); REQUIRE(tc2.fps() == vtm::fps::default_value());
        INFO("tc3 fps: "    << tc3.fps()); REQUIRE(tc3.fps() == vtm::fps::default_value());
        INFO("tc4 fps: "    << tc4.fps()); REQUIRE(tc4.fps() == vtm::fps::default_value());

        // conditions for test
        INFO("tc1 hours: "     << tc1.hours());     CHECK(tc1.hours()     == 12);
        INFO("tc1 minutes: "   << tc1.minutes());   CHECK(tc1.minutes()   == 34);
        INFO("tc1 seconds: "   << tc1.seconds());   CHECK(tc1.seconds()   == 56);
        INFO("tc1 frames: "    << tc1.frames());    CHECK(tc1.frames()    == 12);
        INFO("tc1 subframes: " << tc1.subframes()); CHECK(tc1.subframes() == 34);

        INFO("tc2 hours: "     << tc2.hours());     CHECK(tc2.hours()     == 12);
        INFO("tc2 minutes: "   << tc2.minutes());   CHECK(tc2.minutes()   == 34);
        INFO("tc2 seconds: "   << tc2.seconds());   CHECK(tc2.seconds()   == 56);
        INFO("tc2 frames: "    << tc2.frames());    CHECK(tc2.frames()    == 12);
        INFO("tc2 subframes: " << tc2.subframes()); CHECK(tc2.subframes() == 34);

        INFO("tc3 hours: "     << tc3.hours());     CHECK(tc3.hours()     == 12);
        INFO("tc3 minutes: "   << tc3.minutes());   CHECK(tc3.minutes()   == 34);
        INFO("tc3 seconds: "   << tc3.seconds());   CHECK(tc3.seconds()   == 56);
        INFO("tc3 frames: "    << tc3.frames());    CHECK(tc3.frames()    == 12);
        INFO("tc3 subframes: " << tc3.subframes()); CHECK(tc3.subframes() == 34);

        INFO("tc4 hours: "     << tc4.hours());     CHECK(tc4.hours()     == 12);
        INFO("tc4 minutes: "   << tc4.minutes());   CHECK(tc4.minutes()   == 34);
        INFO("tc4 seconds: "   << tc4.seconds());   CHECK(tc4.seconds()   == 56);
        INFO("tc4 frames: "    << tc4.frames());    CHECK(tc4.frames()    == 12);
        INFO("tc4 subframes: " << tc4.subframes()); CHECK(tc4.subframes() == 34);
    }
}

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION Signed Type Conversion Tests --
//
///////////////////////////////////////////////////////////////////////////

TEST_CASE("vtm::timecode signed type conversions", "[chrono][timecode][conversion][number]")
{
    SECTION("signed type conversion returns correct ticks for each TC group") {
        // source objects
        vtm::timecode tc1{};

        // destination object
        vtm::timecode::signed_type _int1 = 0;

        // pre-conditions for test
        INFO("tc1 fps: " << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());

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

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION String Conversion Tests --
//
///////////////////////////////////////////////////////////////////////////

TEST_CASE("vtm::timecode string conversions", "[chrono][timecode][conversion][string]")
{
    SECTION("string representation corresponds to default object") {
        // default object
        vtm::timecode tc1{};
        tc1.enable_extended_string(true);
        std::string str1 = tc1;

        // pre-conditions for test
        INFO("tc1 fps: "    << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());

        // conditions for test
        INFO("tc1 string: " << str1);      CHECK(str1 == "00:00:00:00.00");
    }

    SECTION("string groups correspond to scalar values") {
        // default object
        vtm::timecode tc1{};
        tc1.enable_extended_string(true);

        // pre-conditions for test
        INFO("tc1 fps: "    << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());

        // conditions for test
        tc1.set_hours(59);
        tc1.set_minutes(42);
        tc1.set_seconds(31);
        tc1.set_frames(12);
        tc1.set_subframes(69);
        std::string str1 = tc1;
        INFO("tc2 string: " << str1);      REQUIRE(str1 == "59:42:31:12.69");
    }

    SECTION("string groups correspond to cascaded scalar values") {
        // default object
        vtm::timecode tc1{};
        tc1.enable_extended_string(true);

        // pre-conditions for test
        INFO("tc1 fps: "    << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());

        // conditions for test
        tc1.set_fps(vtm::fps::fps_25);
        tc1.set_hours(4);                // 4 hours
        tc1.set_minutes(2 * 60 + 42);    // 2 hours   , 42 minutes
        tc1.set_seconds(6 * 60 + 31);    // 6 minutes , 31 seconds
        tc1.set_frames(9 * 25 + 12);     // 9 seconds , 12 frames
        tc1.set_subframes(4 * 100 + 69); // 4 frames  , 69 subframes
        std::string str2 = tc1;
        INFO("tc2 string: " << str2);      REQUIRE(str2 == "02:06:09:04.69");
    }

    SECTION("string assignment returns valid timecode") {
         // default objects
        vtm::timecode tc1{};
        vtm::timecode tc2{};
        vtm::timecode tc3{};

        tc1 = "12:34:56:12.34";
        tc2 = std::string{ "12:34:56:12.34" };
        tc3 = std::string_view{ "12:34:56:12.34" };

        // pre-conditions for test
        INFO("tc1 fps: "    << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());
        INFO("tc2 fps: "    << tc2.fps()); REQUIRE(tc2.fps() == vtm::fps::default_value());
        INFO("tc3 fps: "    << tc3.fps()); REQUIRE(tc3.fps() == vtm::fps::default_value());

        // conditions for test
        INFO("tc1 hours: "     << tc1.hours());     CHECK(tc1.hours()     == 12);
        INFO("tc1 minutes: "   << tc1.minutes());   CHECK(tc1.minutes()   == 34);
        INFO("tc1 seconds: "   << tc1.seconds());   CHECK(tc1.seconds()   == 56);
        INFO("tc1 frames: "    << tc1.frames());    CHECK(tc1.frames()    == 12);
        INFO("tc1 subframes: " << tc1.subframes()); CHECK(tc1.subframes() == 34);

        INFO("tc2 hours: "     << tc2.hours());     CHECK(tc2.hours()     == 12);
        INFO("tc2 minutes: "   << tc2.minutes());   CHECK(tc2.minutes()   == 34);
        INFO("tc2 seconds: "   << tc2.seconds());   CHECK(tc2.seconds()   == 56);
        INFO("tc2 frames: "    << tc2.frames());    CHECK(tc2.frames()    == 12);
        INFO("tc2 subframes: " << tc2.subframes()); CHECK(tc2.subframes() == 34);

        INFO("tc3 hours: "     << tc3.hours());     CHECK(tc3.hours()     == 12);
        INFO("tc3 minutes: "   << tc3.minutes());   CHECK(tc3.minutes()   == 34);
        INFO("tc3 seconds: "   << tc3.seconds());   CHECK(tc3.seconds()   == 56);
        INFO("tc3 frames: "    << tc3.frames());    CHECK(tc3.frames()    == 12);
        INFO("tc3 subframes: " << tc3.subframes()); CHECK(tc3.subframes() == 34);
    }
}

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION Equality & Comparison Tests --
//
///////////////////////////////////////////////////////////////////////////

TEST_CASE("vtm::timecode equality & comparison", "[timecode][chrono][comparison][equality][ordering]")
{
    SECTION("equivalent timecode objects with same fps returns true") {
        // default objects
        vtm::timecode tc1{"12:34:56:12.34"};
        vtm::timecode tc2{"12:34:56:12.34"};

        // pre-conditions for test
        INFO("tc1 fps: "    << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());
        INFO("tc2 fps: "    << tc2.fps()); REQUIRE(tc2.fps() == vtm::fps::default_value());

        // conditions for test
        INFO("tc1 == tc2: "     << (tc1 == tc2)); CHECK(tc1 == tc2);
        INFO("tc1 != tc2: "     << (tc1 != tc2)); CHECK_FALSE(tc1 != tc2);
    }

    SECTION("inequivalent timecode objects with same fps returns false") {
        // default objects
        vtm::timecode tc1{"12:34:56:12.34"};
        vtm::timecode tc2{"34:12:56:12.34"};

        // pre-conditions for test
        INFO("tc1 fps: "    << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());
        INFO("tc2 fps: "    << tc2.fps()); REQUIRE(tc2.fps() == vtm::fps::default_value());

        // conditions for test
        INFO("tc1 == tc2: "     << (tc1 == tc2)); CHECK_FALSE(tc1 == tc2);
        INFO("tc1 != tc2: "     << (tc1 != tc2)); CHECK(tc1 != tc2);
    }

    SECTION("equivalent timecode objects with different fps returns false") {
        // default objects
        vtm::timecode tc1{"12:34:56:12.34"};
        vtm::timecode tc2{"12:34:56:12.34"};
        tc2.set_fps(vtm::fps::fps_30);

        // pre-conditions for test
        INFO("tc1 fps: "    << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());
        INFO("tc2 fps: "    << tc2.fps()); REQUIRE(tc2.fps() == vtm::fps::fps_30);

        // conditions for test
        INFO("tc1 == tc2: "     << (tc1 == tc2)); CHECK_FALSE(tc1 == tc2);
        INFO("tc1 != tc2: "     << (tc1 != tc2)); CHECK(tc1 != tc2);
    }

    SECTION("inequivalent timecode objects with different fps returns false") {
        // default objects
        vtm::timecode tc1{"12:34:56:12.34"};
        vtm::timecode tc2{"34:12:56:12.34"};
        tc2.set_fps(vtm::fps::fps_30);

        // pre-conditions for test
        INFO("tc1 fps: "    << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());
        INFO("tc2 fps: "    << tc2.fps()); REQUIRE(tc2.fps() == vtm::fps::fps_30);

        // conditions for test
        INFO("tc1 == tc2: "     << (tc1 == tc2)); CHECK_FALSE(tc1 == tc2);
        INFO("tc1 != tc2: "     << (tc1 != tc2)); CHECK(tc1 != tc2);
    }

    SECTION("equivalent timecode object and integral returns true") {
        // default objects
        vtm::timecode tc1{"12:34:56:12.34"};
        int _int1 = (12 * 60 * 60 * 25 * 100) +
                    (34 * 60 * 25 * 100) +
                    (56 * 25 * 100) +
                    (12 * 100) + 34;

        // pre-conditions for test
        INFO("tc1 fps: "      << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());

        // conditions for test
        INFO("tc1 ticks: "    << (tc1.ticks()));
        INFO("tc1 == _int1: " << (tc1 == _int1)); CHECK(tc1 == _int1);
        INFO("tc1 != _int1: " << (tc1 != _int1)); CHECK_FALSE(tc1 != _int1);
    }

    SECTION("timecode objects with same fps returns correct ordering") {
        // default objects
        vtm::timecode tc1{"12:34:56:12.34"};
        vtm::timecode tc2{"12:34:56:12.34"};
        vtm::timecode tc3{"00:34:56:12.34"};
        vtm::timecode tc4{"34:34:56:12.34"};

        // pre-conditions for test
        INFO("tc1 fps: "      << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());
        INFO("tc2 fps: "      << tc2.fps()); REQUIRE(tc2.fps() == vtm::fps::default_value());
        INFO("tc3 fps: "      << tc3.fps()); REQUIRE(tc3.fps() == vtm::fps::default_value());
        INFO("tc4 fps: "      << tc4.fps()); REQUIRE(tc4.fps() == vtm::fps::default_value());

        // conditions for test
        INFO("tc1 <= tc2: " << (tc1 <= tc2)); CHECK(tc1 <= tc2);
        INFO("tc1 >= tc2: " << (tc1 >= tc2)); CHECK(tc1 >= tc2);
        INFO("tc1 < tc2: "  << (tc1 < tc2));  CHECK_FALSE(tc1 < tc2);
        INFO("tc1 > tc2: "  << (tc1 > tc2));  CHECK_FALSE(tc1 > tc2);

        INFO("tc1 <= tc3: " << (tc1 <= tc3)); CHECK_FALSE(tc1 <= tc3);
        INFO("tc1 >= tc3: " << (tc1 >= tc3)); CHECK(tc1 >= tc3);
        INFO("tc1 < tc3: "  << (tc1 < tc3));  CHECK_FALSE(tc1 < tc3);
        INFO("tc1 > tc3: "  << (tc1 > tc3));  CHECK(tc1 > tc3);

        INFO("tc1 <= tc4: " << (tc1 <= tc4)); CHECK(tc1 <= tc4);
        INFO("tc1 >= tc4: " << (tc1 >= tc4)); CHECK_FALSE(tc1 >= tc4);
        INFO("tc1 < tc4: "  << (tc1 < tc4));  CHECK(tc1 < tc4);
        INFO("tc1 > tc4: "  << (tc1 > tc4));  CHECK_FALSE(tc1 > tc4);
    }
}

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//  -- @SECTION Arithmetic Operation Tests --
//
///////////////////////////////////////////////////////////////////////////

TEST_CASE("vtm::timecode arithmetic operations", "[timecode][chrono][arithmetic]")
{
    SECTION("addition of timecode objects with same fps yields correct results") {
        // default objects
        vtm::timecode tc1{"00:00:00:00.00"};
        vtm::timecode tc2{"00:00:00:00.42"};
        vtm::timecode tc3{"00:00:00:12.00"};
        vtm::timecode tc4{"00:00:42:00.00"};
        vtm::timecode tc5{"00:42:00:00.00"};
        vtm::timecode tc6{"42:00:00:00.00"};

        // pre-conditions for test
        INFO("tc1 fps: "      << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());
        INFO("tc2 fps: "      << tc2.fps()); REQUIRE(tc2.fps() == vtm::fps::default_value());
        INFO("tc3 fps: "      << tc3.fps()); REQUIRE(tc3.fps() == vtm::fps::default_value());
        INFO("tc4 fps: "      << tc4.fps()); REQUIRE(tc4.fps() == vtm::fps::default_value());
        INFO("tc5 fps: "      << tc5.fps()); REQUIRE(tc5.fps() == vtm::fps::default_value());
        INFO("tc6 fps: "      << tc6.fps()); REQUIRE(tc6.fps() == vtm::fps::default_value());

        // conditions for test
        INFO("tc1 + tc2: " << (tc1 + tc2).ticks()); CHECK(tc1 + tc2 == 42);
        INFO("tc1 + tc3: " << (tc1 + tc3).ticks()); CHECK(tc1 + tc3 == (12 * 100));
        INFO("tc1 + tc4: " << (tc1 + tc4).ticks()); CHECK(tc1 + tc4 == (42 * 25 * 100));
        INFO("tc1 + tc5: " << (tc1 + tc5).ticks()); CHECK(tc1 + tc5 == (42 * 60 * 25 * 100));
        INFO("tc1 + tc6: " << (tc1 + tc6).ticks()); CHECK(tc1 + tc6 == (42 * 60 * 60 * 25 * 100));
        INFO("tc1 + all: " << (tc1 + tc2 + tc3 + tc4 + tc5 + tc6).ticks());
        CHECK((tc1 + tc2 + tc3 + tc4 + tc5 + tc6) == (42 * 60 * 60 * 25 * 100) + (42 * 60 * 25 * 100) + (42 * 25 * 100) + (12 * 100) + (42));
    }

    SECTION("subtraction of timecode objects with same fps yields correct results") {
        // default objects
        vtm::timecode tc1{"42:42:42:12.42"};
        vtm::timecode tc2{"00:00:00:00.42"};
        vtm::timecode tc3{"00:00:00:12.00"};
        vtm::timecode tc4{"00:00:42:00.00"};
        vtm::timecode tc5{"00:42:00:00.00"};
        vtm::timecode tc6{"42:00:00:00.00"};

        // pre-conditions for test
        INFO("tc1 fps: "      << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());
        INFO("tc2 fps: "      << tc2.fps()); REQUIRE(tc2.fps() == vtm::fps::default_value());
        INFO("tc3 fps: "      << tc3.fps()); REQUIRE(tc3.fps() == vtm::fps::default_value());
        INFO("tc4 fps: "      << tc4.fps()); REQUIRE(tc4.fps() == vtm::fps::default_value());
        INFO("tc5 fps: "      << tc5.fps()); REQUIRE(tc5.fps() == vtm::fps::default_value());
        INFO("tc6 fps: "      << tc6.fps()); REQUIRE(tc6.fps() == vtm::fps::default_value());

        // conditions for test
        INFO("tc1 - tc2: " << (tc1 - tc2).ticks()); CHECK(tc1 - tc2 == ((42 * 60 * 60 * 25 * 100) + (42 * 60 * 25 * 100) + (42 * 25 * 100) + (12 * 100)));
        INFO("tc1 - tc3: " << (tc1 - tc3).ticks()); CHECK(tc1 - tc3 == ((42 * 60 * 60 * 25 * 100) + (42 * 60 * 25 * 100) + (42 * 25 * 100) + (42)));
        INFO("tc1 - tc4: " << (tc1 - tc4).ticks()); CHECK(tc1 - tc4 == ((42 * 60 * 60 * 25 * 100) + (42 * 60 * 25 * 100) + (12 * 100) + (42)));
        INFO("tc1 - tc5: " << (tc1 - tc5).ticks()); CHECK(tc1 - tc5 == ((42 * 60 * 60 * 25 * 100) + (42 * 25 * 100) + (12 * 100) + (42)));
        INFO("tc1 - tc6: " << (tc1 - tc6).ticks()); CHECK(tc1 - tc6 == ((42 * 60 * 25 * 100) + (42 * 25 * 100) + (12 * 100) + (42)));
        INFO("tc1 - all: " << (tc1 - tc2 - tc3 - tc4 - tc5 - tc6).ticks()); CHECK((tc1 - tc2 - tc3 - tc4 - tc5 - tc6) == 0);
    }

    SECTION("addition assignment of timecode objects with same fps yields correct results") {
        // default objects
        vtm::timecode tc1{"00:00:00:00.00"};
        vtm::timecode tc2{"00:00:00:00.42"};
        vtm::timecode tc3{"00:00:00:12.00"};
        vtm::timecode tc4{"00:00:42:00.00"};
        vtm::timecode tc5{"00:42:00:00.00"};
        vtm::timecode tc6{"42:00:00:00.00"};

        // pre-conditions for test
        INFO("tc1 fps: "      << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());
        INFO("tc2 fps: "      << tc2.fps()); REQUIRE(tc2.fps() == vtm::fps::default_value());
        INFO("tc3 fps: "      << tc3.fps()); REQUIRE(tc3.fps() == vtm::fps::default_value());
        INFO("tc4 fps: "      << tc4.fps()); REQUIRE(tc4.fps() == vtm::fps::default_value());
        INFO("tc5 fps: "      << tc5.fps()); REQUIRE(tc5.fps() == vtm::fps::default_value());
        INFO("tc6 fps: "      << tc6.fps()); REQUIRE(tc6.fps() == vtm::fps::default_value());

        // conditions for test
        INFO("tc1 step1: " << tc1.ticks()); tc1 += tc2; CHECK(tc1 == 42);
        INFO("tc1 step2: " << tc1.ticks()); tc1 += tc3; CHECK(tc1 == (12 * 100) + (42));
        INFO("tc1 step3: " << tc1.ticks()); tc1 += tc4; CHECK(tc1 == (42 * 25 * 100) + (12 * 100) + (42));
        INFO("tc1 step4: " << tc1.ticks()); tc1 += tc5; CHECK(tc1 == (42 * 60 * 25 * 100) + (42 * 25 * 100) + (12 * 100) + (42));
        INFO("tc1 step5: " << tc1.ticks()); tc1 += tc6; CHECK(tc1 == (42 * 60 * 60 * 25 * 100) + (42 * 60 * 25 * 100) + (42 * 25 * 100) + (12 * 100) + (42));
    }

    SECTION("subtraction assignment of timecode objects with same fps yields correct results") {
        // default objects
        vtm::timecode tc1{"42:42:42:12.42"};
        vtm::timecode tc2{"00:00:00:00.42"};
        vtm::timecode tc3{"00:00:00:12.00"};
        vtm::timecode tc4{"00:00:42:00.00"};
        vtm::timecode tc5{"00:42:00:00.00"};
        vtm::timecode tc6{"42:00:00:00.00"};

        // pre-conditions for test
        INFO("tc1 fps: "      << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());
        INFO("tc2 fps: "      << tc2.fps()); REQUIRE(tc2.fps() == vtm::fps::default_value());
        INFO("tc3 fps: "      << tc3.fps()); REQUIRE(tc3.fps() == vtm::fps::default_value());
        INFO("tc4 fps: "      << tc4.fps()); REQUIRE(tc4.fps() == vtm::fps::default_value());
        INFO("tc5 fps: "      << tc5.fps()); REQUIRE(tc5.fps() == vtm::fps::default_value());
        INFO("tc6 fps: "      << tc6.fps()); REQUIRE(tc6.fps() == vtm::fps::default_value());

        // conditions for test
        INFO("tc1 step1: " << tc1.ticks()); tc1 -= tc2; CHECK(tc1 == (42 * 60 * 60 * 25 * 100) + (42 * 60 * 25 * 100) + (42 * 25 * 100) + (12 * 100));
        INFO("tc1 step2: " << tc1.ticks()); tc1 -= tc3; CHECK(tc1 == (42 * 60 * 60 * 25 * 100) + (42 * 60 * 25 * 100) + (42 * 25 * 100));
        INFO("tc1 step3: " << tc1.ticks()); tc1 -= tc4; CHECK(tc1 == (42 * 60 * 60 * 25 * 100) + (42 * 60 * 25 * 100));
        INFO("tc1 step4: " << tc1.ticks()); tc1 -= tc5; CHECK(tc1 == (42 * 60 * 60 * 25 * 100));
        INFO("tc1 step5: " << tc1.ticks()); tc1 -= tc6; CHECK(tc1 == 0);
    }

    SECTION("multiplication of timecode objects with same fps yields correct results") {
        // default objects
        vtm::timecode tc1{"00:00:00:00.01"};
        vtm::timecode tc2{"00:00:00:00.42"};
        vtm::timecode tc3{"00:00:00:12.00"};
        vtm::timecode tc4{"00:00:42:00.00"};
        vtm::timecode tc5{"00:42:00:00.00"};
        vtm::timecode tc6{"42:00:00:00.00"};

        // pre-conditions for test
        INFO("tc1 fps: "      << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());
        INFO("tc2 fps: "      << tc2.fps()); REQUIRE(tc2.fps() == vtm::fps::default_value());
        INFO("tc3 fps: "      << tc3.fps()); REQUIRE(tc3.fps() == vtm::fps::default_value());
        INFO("tc4 fps: "      << tc4.fps()); REQUIRE(tc4.fps() == vtm::fps::default_value());
        INFO("tc5 fps: "      << tc5.fps()); REQUIRE(tc5.fps() == vtm::fps::default_value());
        INFO("tc6 fps: "      << tc6.fps()); REQUIRE(tc6.fps() == vtm::fps::default_value());

        // conditions for test
        INFO("tc1 * tc2: " << (tc1 * vtm::timecode{0}).ticks()); CHECK(tc1 * vtm::timecode{0} == 0);
        INFO("tc1 * tc2: " << (tc1 * tc2).ticks()); CHECK(tc1 * tc2 == 42);
        INFO("tc1 * tc3: " << (tc1 * tc3).ticks()); CHECK(tc1 * tc3 == (12 * 100));
        INFO("tc1 * tc4: " << (tc1 * tc4).ticks()); CHECK(tc1 * tc4 == (42 * 25 * 100));
        INFO("tc1 * tc5: " << (tc1 * tc5).ticks()); CHECK(tc1 * tc5 == (42 * 60 * 25 * 100));
        INFO("tc1 * tc6: " << (tc1 * tc6).ticks()); CHECK(tc1 * tc6 == (42 * 60 * 60 * 25 * 100));
    }

    SECTION("division of timecode objects with same fps yields correct results") {
        // default objects
        vtm::timecode tc1{"00:00:00:00.02"};
        vtm::timecode tc2{"00:00:00:00.42"};
        vtm::timecode tc3{"00:00:00:12.00"};
        vtm::timecode tc4{"00:00:42:00.00"};
        vtm::timecode tc5{"00:42:00:00.00"};
        vtm::timecode tc6{"42:00:00:00.00"};

        // pre-conditions for test
        INFO("tc1 fps: "      << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());
        INFO("tc2 fps: "      << tc2.fps()); REQUIRE(tc2.fps() == vtm::fps::default_value());
        INFO("tc3 fps: "      << tc3.fps()); REQUIRE(tc3.fps() == vtm::fps::default_value());
        INFO("tc4 fps: "      << tc4.fps()); REQUIRE(tc4.fps() == vtm::fps::default_value());
        INFO("tc5 fps: "      << tc5.fps()); REQUIRE(tc5.fps() == vtm::fps::default_value());
        INFO("tc6 fps: "      << tc6.fps()); REQUIRE(tc6.fps() == vtm::fps::default_value());

        // conditions for test
        INFO("tc2 / tc1: " << (tc2 / tc1).ticks()); CHECK(tc2 / tc1 == 21);
        INFO("tc3 / tc1: " << (tc3 / tc1).ticks()); CHECK(tc3 / tc1 == (6 * 100));
        INFO("tc4 / tc1: " << (tc4 / tc1).ticks()); CHECK(tc4 / tc1 == (21 * 25 * 100));
        INFO("tc5 / tc1: " << (tc5 / tc1).ticks()); CHECK(tc5 / tc1 == (21 * 60 * 25 * 100));
        INFO("tc6 / tc1: " << (tc6 / tc1).ticks()); CHECK(tc6 / tc1 == (21 * 60 * 60 * 25 * 100));
    }

    SECTION("pre-increment operator yields correct results") {
        // default objects
        vtm::timecode tc1{"00:00:00:00.00"};
        std::size_t iterations = (59 * 25 * 100) + (24 * 100) + 99;
        for (; tc1 < iterations; ++tc1);

        // pre-conditions for test
        INFO("tc1 fps: " << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());


        // conditions for test
        INFO("tc1 value: " << tc1.ticks()); CHECK(tc1 == iterations);
    }

    SECTION("post-increment operator yields correct results") {
        // default objects
        vtm::timecode tc1{"00:00:00:00.00"};
        std::size_t iterations = (59 * 25 * 100) + (24 * 100) + 99;
        for (; tc1 < iterations; tc1++);

        // pre-conditions for test
        INFO("tc1 fps: " << tc1.fps()); REQUIRE(tc1.fps() == vtm::fps::default_value());


        // conditions for test
        INFO("tc1 value: " << tc1.ticks()); CHECK(tc1 == iterations);
    }
}

///////////////////////////////////////////////////////////////////////////
