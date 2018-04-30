cmake_minimum_required(VERSION 3.10)
project(option C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

include(deps/panic/panic.cmake)

add_library(option sources/option.h sources/option.c)
target_link_libraries(option PRIVATE panic)
target_include_directories(option PRIVATE deps)

add_executable(main examples/main.c)
target_link_libraries(main PRIVATE option)
target_include_directories(main PRIVATE sources)
