#define CATCH_CONFIG_MAIN
#include <Catch2/catch_all.hpp>
#include "catch2/catch_message.hpp"
#include "catch2/catch_test_macros.hpp"
#include "timecode.hpp"
#include <string>
#include <string_view>

TEST_CASE("vtm::timecode Initialization", "[timecode][chrono][initialization]")
{
    vtm::timecode tc1{};
    REQUIRE(1 == 1);
}
