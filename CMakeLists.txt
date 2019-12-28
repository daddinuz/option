cmake_minimum_required(VERSION 3.10)
project(option C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wpedantic -pedantic-errors")

# dependencies
include_directories(deps)
include(deps/panic/build.cmake)
include(deps/traits-unit/build.cmake)

# archive
include_directories(sources)

# examples
include(examples/build.cmake)

# tests
include(tests/unit/build.cmake)
