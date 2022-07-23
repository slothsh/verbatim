#define CATCH_CONFIG_MAIN
#include <Catch2/catch_all.hpp>
#include "edlfile.hpp"

TEST_CASE("EDL File Initialization", "[EDL File]")
{
    // @SECTION Test variables
    constexpr std::string_view txt_path_good{"./resources/txt/helloworld.txt"};
    constexpr std::string_view txt_path_bad{"./resources/txt/foo.txt"};

    // @SECTION Constructors
    vtm::avidpt_edl edl_file_default{};
    vtm::avidpt_edl edl_file_path_good{txt_path_good};
    vtm::avidpt_edl edl_file_path_bad{txt_path_bad};

    // @SECTION Standard library implicit conversion overloads
    /* REQUIRE(edl_file.display()           == "EDL File Empty"); */
    /* REQUIRE(std::string{edl_file}        == "EDL File Empty"); */
    /* REQUIRE(std::string_view{ edl_file } == "EDL File Empty"); */
}
