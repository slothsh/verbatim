cmake_minimum_required(VERSION 3.15.0)
project(verbatim VERSION 0.0.1 LANGUAGES CXX)
message(STATUS "Project source path: ${CMAKE_SOURCE_DIR}")

# Global compiler configuration
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (APPLE)
    set(CMAKE_CXX_STANDARD 20)
else()
    set(CMAKE_CXX_STANDARD 23)
endif()

# Comiler Warnings & Run time library
if (MSVC)
    if (BUILD_CONFIG STREQUAL "debug")
        set(LINK_RUNTIME_LIB "/MTd")
    else()
        set(LINK_RUNTIME_LIB "/MT")
    endif()

    set(COMPILE_WARNINGS "/W4")
    set(LINK_RUNTIME_EXCEPTIONS "/EHsc")
    add_compile_options(${LINK_RUNTIME_LIB} ${COMPILE_WARNINGS} ${LINK_RUNTIME_EXCEPTIONS})
else()
    set(COMPILE_WARNINGS "-Wall -Wpedantic")
    add_compile_options(${LINK_RUNTIME_LIB} ${COMPILE_WARNINGS})
endif()

message(STATUS "Setting compiler warnings to ${COMPILE_WARNINGS}")
message(STATUS "Linking MSVC run time library to ${LINK_RUNTIME_LIB}")

# Build Third-Party Dependencies via vcpkg
find_package(fmt REQUIRED)
find_package(magic_enum REQUIRED)

# Include local and third-party headers
include_directories(
    include
)

# Configure tests
enable_testing()
add_subdirectory(tests)
