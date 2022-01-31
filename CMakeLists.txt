cmake_minimum_required(VERSION 3.15.0)

# Global compiler configuration
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build Third-Party Dependencies
add_subdirectory(third-party/fmt)
add_subdirectory(third-party/Catch2)

# Include third-party headers
include_directories(
    third-party/fmt/include
    third-party/magic_enum/include
    third-party/cxxopts/include
    third-party/Catch2/src
)
    
# Configure primary build for project
project(verbatim VERSION 0.0.1 LANGUAGES CXX)

add_executable(helloverbatim)
target_sources(helloverbatim PUBLIC src/helloverbatim.cxx)
target_link_libraries(helloverbatim PRIVATE fmt)

# Configure tests
include(third-party/Catch2/extras/Catch.cmake)
find_package(Catch2 3 REQUIRED)
include(CTest)
add_executable(testhelloverbatim tests/helloverbatim.test.cxx)
target_link_libraries(testhelloverbatim PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(testhelloverbatim)