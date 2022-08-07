# Test configuration
find_package(Catch2 REQUIRED)
find_path(CATCH2_CMAKE NAMES Catch2Config.cmake PATH_SUFFIX Catch2)

# Library tests
add_executable(edlfile.test edlfile.test.cpp)
add_executable(timecode_int.test timecode_int.test.cpp)
add_executable(timecode_float.test timecode_float.test.cpp)
add_executable(fps.test fps.test.cpp)
add_executable(functional.test functional.test.cpp)

target_link_libraries(edlfile.test PRIVATE Catch2::Catch2WithMain fmt::fmt magic_enum::magic_enum)
target_link_libraries(timecode_int.test PRIVATE Catch2::Catch2WithMain fmt::fmt magic_enum::magic_enum)
target_link_libraries(timecode_float.test PRIVATE Catch2::Catch2WithMain fmt::fmt magic_enum::magic_enum)
target_link_libraries(fps.test PRIVATE Catch2::Catch2WithMain fmt::fmt magic_enum::magic_enum)
target_link_libraries(functional.test PRIVATE Catch2::Catch2WithMain fmt::fmt magic_enum::magic_enum)

include(CTest)
include(Catch)
catch_discover_tests(edlfile.test)
catch_discover_tests(timecode_int.test)
catch_discover_tests(timecode_float.test)
catch_discover_tests(fps.test)
catch_discover_tests(functional.test)
