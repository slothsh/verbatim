#define CATCH_CONFIG_MAIN
#include <Catch2/catch_all.hpp>
#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_message.hpp"
#include "catch2/catch_test_macros.hpp"
#include "timecode.hpp"
#include <string>
#include <string_view>

TEST_CASE("vtm::timecode arithmetic benchmarks", "[timecode][chrono][arithmetic]")
{
    SECTION("pre-increment operator yields correct results") {
        // source object
        vtm::timecode tc1{};
        std::size_t iterations = (59 * 25 * 100) + (24 * 100) + 99;

        // control test
        BENCHMARK("pre-increment control with size_t to 1 minute") {
            for (std::size_t i = 0; i < iterations; ++i);
            return 0;
        };

        // benchmark
        BENCHMARK("pre-increment to 1 minute") {
            for (; tc1 < iterations; ++tc1);
            return 0;
        };
    }
}
