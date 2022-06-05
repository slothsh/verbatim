cmake_minimum_required(VERSION 3.15.0)
project(verbatim VERSION 0.0.1 LANGUAGES CXX)
message(STATUS "Project source path: ${CMAKE_SOURCE_DIR}")

# Global compiler configuration
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(COMPILE_WARNINGS "/W4")
set(LINK_RUNTIME_LIB "/MT")

# Comiler Warnings & Run time library
if (MSVC)
    message(STATUS "Setting compiler warnings to ${COMPILE_WARNINGS}")
    message(STATUS "Linking MSVC run time library to ${LINK_RUNTIME_LIB}")
    add_compile_options(${LINK_RUNTIME_LIB} ${COMPILE_WARNINGS})
endif()

# Build Third-Party Dependencies
add_subdirectory(third-party/fmt)
add_subdirectory(third-party/Catch2)
add_subdirectory(third-party/pugixml)

# Include third-party headers
include_directories(
    third-party/fmt/include
    third-party/magic_enum/include
    third-party/cxxopts/include
    third-party/Catch2/src
    third-party/pugixml/src
    third-party/compile-time-regular-expressions
    include
)

# Configure tests
add_subdirectory(tests)
