# Test configuration
enable_testing()
include(CTest)
include(${CMAKE_SOURCE_DIR}/third-party/Catch2/extras/Catch.cmake)
find_package(Catch2 3 REQUIRED)

# Library tests
add_executable(edlfile.test edlfile.test.cpp)
add_executable(timecode.test timecode.test.cpp)
add_executable(fps.test fps.test.cpp)

target_link_libraries(edlfile.test PRIVATE Catch2::Catch2WithMain fmt)
target_link_libraries(timecode.test PRIVATE Catch2::Catch2WithMain fmt)
target_link_libraries(fps.test PRIVATE Catch2::Catch2WithMain fmt)

catch_discover_tests(edlfile.test)
catch_discover_tests(timecode.test)
catch_discover_tests(fps.test)
