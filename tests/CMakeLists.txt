# Test configuration
enable_testing()
find_package(Catch2 REQUIRED)
find_path(CATCH2_CMAKE NAMES Catch.cmake PATH_SUFFIX Catch2)

# Library tests
add_executable(edlfile.test edlfile.test.cpp)
add_executable(timecode.test timecode.test.cpp)
add_executable(fps.test fps.test.cpp)
add_executable(functional.test functional.test.cpp)

target_link_libraries(edlfile.test PRIVATE Catch2::Catch2WithMain fmt::fmt)
target_link_libraries(timecode.test PRIVATE Catch2::Catch2WithMain fmt::fmt)
target_link_libraries(fps.test PRIVATE Catch2::Catch2WithMain fmt::fmt)
target_link_libraries(functional.test PRIVATE Catch2::Catch2WithMain fmt::fmt)

include(CTest)
include(Catch)
catch_discover_tests(edlfile.test)
catch_discover_tests(timecode.test)
catch_discover_tests(fps.test)
catch_discover_tests(functional.test)
