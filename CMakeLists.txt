cmake_minimum_required(VERSION 3.15.0)
project(verbatim VERSION 0.0.1 LANGUAGES CXX)
message(STATUS "Project source path: ${CMAKE_SOURCE_DIR}")

# Handle enviroment variables
if (NOT BUILD_CONFIG STREQUAL "debug" AND NOT BUILD_CONFIG STREQUAL "release")
    set(BUILD_CONFIG "release")
endif()

if (NOT WARNING_CONFIG STREQUAL "verbose" AND NOT WARNING_CONFIG STREQUAL "normal")
    set(WARNING_CONFIG "normal")
endif()

# Global compiler configuration
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
    set(CMAKE_CXX_STANDARD 23)
else()
    set(CMAKE_CXX_STANDARD 20)
endif()

# MSVC compiler parameters
if (MSVC)
    if (${BUILD_CONFIG} STREQUAL "debug")
        set(LINK_RUNTIME_LIB "/MTd")
    else()
        set(LINK_RUNTIME_LIB "/MT")
    endif()

    if (${WARNING_CONFIG} STREQUAL "verbose")
        set(COMPILE_WARNINGS "/W4")
    else()
        set(COMPILE_WARNINGS "/W2")
    endif()

    set(LINK_RUNTIME_EXCEPTIONS "/EHsc")

    add_compile_options(${LINK_RUNTIME_LIB} ${COMPILE_WARNINGS} ${LINK_RUNTIME_EXCEPTIONS})

    message(STATUS "Setting compiler warnings to ${COMPILE_WARNINGS}")
    message(STATUS "Linking run time library to ${LINK_RUNTIME_LIB}")
    message(STATUS "Setting exception handling model to ${LINK_RUNTIME_LIB}")

# Unix compiler parameters
else()
    add_compile_options("-Wall" "-Wpedantic")
endif()

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
