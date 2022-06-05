#include <Catch2/catch_all.hpp>
#include "io.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <string_view>

using namespace vtm::io;

static constexpr std::string_view file_path{ "C:/Users/snowf/dev/projects/verbatim/resources/txt/helloworld.txt" };

TEST_CASE("take_one(): ifstream text parsing", "[text][input][parsing]")
{
    std::ifstream file(file_path.data(), FSTREAM_IN);

    // Test sequence
    const auto str1 = take_one(file);
    const auto str2 = take_one(file);
    const auto str3 = take_one(file);
    const auto str4 = take_one(file);
    const auto str5 = take_one(file);

    INFO("str1 value: " << *str1);
    INFO("str2 value: " << *str2);
    INFO("str3 value: " << *str3);
    INFO("str4 value: " << *str4);
    INFO("str5 value: " << *str5);

    REQUIRE(*str1 == "H");
    REQUIRE(*str2 == "e");
    REQUIRE(*str3 == "l");
    REQUIRE(*str4 == "l");
    REQUIRE(*str5 == "o");
}

TEST_CASE("take_while(): ifstream text parsing", "[text][input][parsing]")
{
    std::ifstream file(file_path.data(), FSTREAM_IN);

    const auto e = [](const auto& arg) { return arg != "," && arg != " " && arg != "!" && arg != "\r" && arg != "\n"; };
    const auto f = [](const auto& arg) { return arg != ","; };
    const auto g = [](const auto& arg) { return arg != " "; };
    const auto h = [](const auto& arg) { return arg != "!"; };
    const auto i = [](const auto& arg) { return arg != "\r"; };
    const auto j = [](const auto& arg) { return arg != "\n"; };

    const auto str1 = take_while(file, e);

    file.clear();
    file.seekg(0);
    const auto str2 = take_while(file, f, g, h, i, j);

    INFO("str1 value: " << str1);
    INFO("str2 value: " << str2);

    REQUIRE(str1 == "Hello");
    REQUIRE(str2 == "Hello");
}

TEST_CASE("skip(): ifstream text parsing", "[text][parsing]")
{
    std::ifstream file(file_path.data(), FSTREAM_IN);

    const auto e = [](const auto& arg) { return arg != "," && arg != " " && arg != "!" && arg != "\r" && arg != "\n"; };
    const auto f = [](const auto& arg) { return arg != ","; };
    const auto g = [](const auto& arg) { return arg != " "; };
    const auto h = [](const auto& arg) { return arg != "!"; };
    const auto i = [](const auto& arg) { return arg != "\r"; };
    const auto j = [](const auto& arg) { return arg != "\n"; };

    const auto str1 = skip(file, e);

    file.clear();
    file.seekg(0);
    const auto str2 = skip(file, f, g, h, i, j);

    INFO("str1 value: " << str1);
    INFO("str2 value: " << str2);

    REQUIRE(str1 == "HelloWorld");
    REQUIRE(str2 == "HelloWorld");
}
