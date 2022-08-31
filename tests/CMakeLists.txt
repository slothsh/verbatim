# Test configuration
find_package(Catch2 REQUIRED)
find_path(CATCH2_CMAKE NAMES Catch2Config.cmake PATH_SUFFIX Catch2)

# Handle compiler test definitions
if (ENABLE_STATIC_TESTS STREQUAL "on" OR ENABLE_STATIC_TESTS STREQUAL "true" OR ENABLE_STATIC_TESTS STREQUAL "1")
    message(STATUS "Enabling static tests for this build")
    add_compile_definitions(ENABLE_STATIC_TESTS)
endif()

# Library tests
add_executable(timecode_example.test timecode_example.test.cpp)
add_executable(edlfile.test edlfile.test.cpp)
add_executable(timecode_int.test timecode_int.test.cpp)
add_executable(timecode_int.bench timecode_int.bench.cpp)
add_executable(timecode_float.test timecode_float.test.cpp)
add_executable(fps.test fps.test.cpp)
add_executable(functional.test functional.test.cpp)

target_link_libraries(timecode_example.test PRIVATE fmt::fmt magic_enum::magic_enum)
target_link_libraries(edlfile.test PRIVATE Catch2::Catch2WithMain fmt::fmt magic_enum::magic_enum)
target_link_libraries(timecode_int.test PRIVATE Catch2::Catch2WithMain fmt::fmt magic_enum::magic_enum)
target_link_libraries(timecode_int.bench PRIVATE Catch2::Catch2WithMain fmt::fmt magic_enum::magic_enum)
target_link_libraries(timecode_float.test PRIVATE Catch2::Catch2WithMain fmt::fmt magic_enum::magic_enum)
target_link_libraries(fps.test PRIVATE Catch2::Catch2WithMain fmt::fmt magic_enum::magic_enum)
target_link_libraries(functional.test PRIVATE Catch2::Catch2WithMain fmt::fmt magic_enum::magic_enum)

include(CTest)
include(Catch)
catch_discover_tests(edlfile.test)
catch_discover_tests(timecode_int.test)
catch_discover_tests(timecode_int.bench)
catch_discover_tests(timecode_float.test)
catch_discover_tests(fps.test)
catch_discover_tests(functional.test)
