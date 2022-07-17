
#include <tuple>
#include <utility>
#include <Catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include "functional.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define AT(T, I) std::get<I>(T)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("vtm::functional rotl()", "[functional][generic][constexpr][tuple][rotl]")
{
    SECTION("full tuple rotation using rotl()")
    {
        using namespace vtm::functional;
        constexpr std::tuple t1{ 1, 2, 3, 4, 5 };

        {
            constexpr auto t2 = rotl(t1);
            INFO("rotl pass 1 on t2: " << AT(t2, 0)); INFO("rotl pass 1 on t2: " << AT(t2, 1));
            INFO("rotl pass 1 on t2: " << AT(t2, 2)); INFO("rotl pass 1 on t2: " << AT(t2, 3));
            INFO("rotl pass 1 on t2: " << AT(t2, 4));
            REQUIRE(AT(t2, 0) == 2); REQUIRE(AT(t2, 1) == 3);
            REQUIRE(AT(t2, 2) == 4); REQUIRE(AT(t2, 3) == 5);
            REQUIRE(AT(t2, 4) == 1);
        }

        {
            constexpr auto t2 = rotl(rotl(t1));
            INFO("rotl pass 2 on t2: " << AT(t2, 0)); INFO("rotl pass 2 on t2: " << AT(t2, 1));
            INFO("rotl pass 2 on t2: " << AT(t2, 2)); INFO("rotl pass 2 on t2: " << AT(t2, 3));
            INFO("rotl pass 2 on t2: " << AT(t2, 4));
            REQUIRE(AT(t2, 0) == 3); REQUIRE(AT(t2, 1) == 4);
            REQUIRE(AT(t2, 2) == 5); REQUIRE(AT(t2, 3) == 1);
            REQUIRE(AT(t2, 4) == 2);
        }

        {
            constexpr auto t2 = rotl(rotl(rotl(t1)));
            INFO("rotl pass 3 on t2: " << AT(t2, 0)); INFO("rotl pass 3 on t2: " << AT(t2, 1));
            INFO("rotl pass 3 on t2: " << AT(t2, 2)); INFO("rotl pass 3 on t2: " << AT(t2, 3));
            INFO("rotl pass 3 on t2: " << AT(t2, 4));
            REQUIRE(AT(t2, 0) == 4); REQUIRE(AT(t2, 1) == 5);
            REQUIRE(AT(t2, 2) == 1); REQUIRE(AT(t2, 3) == 2);
            REQUIRE(AT(t2, 4) == 3);
        }

        {
            constexpr auto t2 = rotl(rotl(rotl(rotl(t1))));
            INFO("rotl pass 4 on t2: " << AT(t2, 0)); INFO("rotl pass 4 on t2: " << AT(t2, 1));
            INFO("rotl pass 4 on t2: " << AT(t2, 2)); INFO("rotl pass 4 on t2: " << AT(t2, 3));
            INFO("rotl pass 4 on t2: " << AT(t2, 4));
            REQUIRE(AT(t2, 0) == 5); REQUIRE(AT(t2, 1) == 1);
            REQUIRE(AT(t2, 2) == 2); REQUIRE(AT(t2, 3) == 3);
            REQUIRE(AT(t2, 4) == 4);
        }

        {
            constexpr auto t2 = rotl(rotl(rotl(rotl(rotl(t1)))));
            INFO("rotl pass 5 on t2: " << AT(t2, 0)); INFO("rotl pass 5 on t2: " << AT(t2, 1));
            INFO("rotl pass 5 on t2: " << AT(t2, 2)); INFO("rotl pass 5 on t2: " << AT(t2, 3));
            INFO("rotl pass 5 on t2: " << AT(t2, 4));
            REQUIRE(AT(t2, 0) == 1); REQUIRE(AT(t2, 1) == 2);
            REQUIRE(AT(t2, 2) == 3); REQUIRE(AT(t2, 3) == 4);
            REQUIRE(AT(t2, 4) == 5);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("vtm::functional rotr()", "[functional][generic][constexpr][tuple][rotr]")
{
    SECTION("full tuple rotation using rotr()")
    {
        using namespace vtm::functional;
        constexpr std::tuple t1{ 1, 2, 3, 4, 5 };

        {
            constexpr auto t2 = rotr(t1);
            INFO("rotr pass 1 on t2: " << AT(t2, 0)); INFO("rotr pass 1 on t2: " << AT(t2, 1));
            INFO("rotr pass 1 on t2: " << AT(t2, 2)); INFO("rotr pass 1 on t2: " << AT(t2, 3));
            INFO("rotr pass 1 on t2: " << AT(t2, 4));
            REQUIRE(AT(t2, 0) == 5); REQUIRE(AT(t2, 1) == 1);
            REQUIRE(AT(t2, 2) == 2); REQUIRE(AT(t2, 3) == 3);
            REQUIRE(AT(t2, 4) == 4);
        }

        {
            constexpr auto t2 = rotr(rotr(t1));
            INFO("rotr pass 2 on t2: " << AT(t2, 0)); INFO("rotr pass 2 on t2: " << AT(t2, 1));
            INFO("rotr pass 2 on t2: " << AT(t2, 2)); INFO("rotr pass 2 on t2: " << AT(t2, 3));
            INFO("rotr pass 2 on t2: " << AT(t2, 4));
            REQUIRE(AT(t2, 0) == 4); REQUIRE(AT(t2, 1) == 5);
            REQUIRE(AT(t2, 2) == 1); REQUIRE(AT(t2, 3) == 2);
            REQUIRE(AT(t2, 4) == 3);
        }

        {
            constexpr auto t2 = rotr(rotr(rotr(t1)));
            INFO("rotr pass 3 on t2: " << AT(t2, 0)); INFO("rotr pass 3 on t2: " << AT(t2, 1));
            INFO("rotr pass 3 on t2: " << AT(t2, 2)); INFO("rotr pass 3 on t2: " << AT(t2, 3));
            INFO("rotr pass 3 on t2: " << AT(t2, 4));
            REQUIRE(AT(t2, 0) == 3); REQUIRE(AT(t2, 1) == 4);
            REQUIRE(AT(t2, 2) == 5); REQUIRE(AT(t2, 3) == 1);
            REQUIRE(AT(t2, 4) == 2);
        }

        {
            constexpr auto t2 = rotr(rotr(rotr(rotr(t1))));
            INFO("rotr pass 4 on t2: " << AT(t2, 0)); INFO("rotr pass 4 on t2: " << AT(t2, 1));
            INFO("rotr pass 4 on t2: " << AT(t2, 2)); INFO("rotr pass 4 on t2: " << AT(t2, 3));
            INFO("rotr pass 4 on t2: " << AT(t2, 4));
            REQUIRE(AT(t2, 0) == 2); REQUIRE(AT(t2, 1) == 3);
            REQUIRE(AT(t2, 2) == 4); REQUIRE(AT(t2, 3) == 5);
            REQUIRE(AT(t2, 4) == 1);
        }

        {
            constexpr auto t2 = rotr(rotr(rotr(rotr(rotr(t1)))));
            INFO("rotr pass 5 on t2: " << AT(t2, 0)); INFO("rotr pass 5 on t2: " << AT(t2, 1));
            INFO("rotr pass 5 on t2: " << AT(t2, 2)); INFO("rotr pass 5 on t2: " << AT(t2, 3));
            INFO("rotr pass 5 on t2: " << AT(t2, 4));
            REQUIRE(AT(t2, 0) == 1); REQUIRE(AT(t2, 1) == 2);
            REQUIRE(AT(t2, 2) == 3); REQUIRE(AT(t2, 3) == 4);
            REQUIRE(AT(t2, 4) == 5);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("vtm::functional swap()", "[functional][generic][constexpr][tuple][rotr]")
{
    SECTION("swap back and forth with swap()")
    {
        using namespace vtm::functional;
        constexpr std::tuple t1{ 1, 2, 3, 4, 5 };

        {
            constexpr auto t2 = swap(t1);
            constexpr auto t3 = swap(t2);

            INFO("swap on t2: " << AT(t2, 0)); INFO("swap on t2: " << AT(t2, 1));
            INFO("swap on t2: " << AT(t2, 2)); INFO("swap on t2: " << AT(t2, 3));
            INFO("swap on t2: " << AT(t2, 4));

            INFO("swap on t3: " << AT(t3, 0)); INFO("swap on t3: " << AT(t3, 1));
            INFO("swap on t3: " << AT(t3, 2)); INFO("swap on t3: " << AT(t3, 3));
            INFO("swap on t3: " << AT(t3, 4));

            REQUIRE(AT(t2, 0) == 2); REQUIRE(AT(t2, 1) == 1);
            REQUIRE(AT(t2, 2) == 3); REQUIRE(AT(t2, 3) == 4);
            REQUIRE(AT(t2, 4) == 5);

            REQUIRE(AT(t3, 0) == 1); REQUIRE(AT(t3, 1) == 2);
            REQUIRE(AT(t3, 2) == 3); REQUIRE(AT(t3, 3) == 4);
            REQUIRE(AT(t3, 4) == 5);
        }
    }
}
