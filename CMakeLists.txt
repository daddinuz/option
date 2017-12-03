cmake_minimum_required(VERSION 3.0)
project(option)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcast-align -Wbad-function-cast")
endif ()

#####
# Variable definitions
###
set(EXAMPLES_PATH ${PROJECT_SOURCE_DIR}/examples)
set(PROJECT_PATH ${PROJECT_SOURCE_DIR}/sources)

file(GLOB EXAMPLE_SOURCES ${EXAMPLES_PATH}/example*.c)
file(GLOB PROJECT_HEADERS ${PROJECT_PATH}/*.h)
file(GLOB PROJECT_SOURCES ${PROJECT_PATH}/*.c)

#####
# Working directories
###
include_directories(${PROJECT_PATH})

#####
# Archive
###
add_library(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})

#####
# Examples
###
foreach (source_file ${EXAMPLE_SOURCES})
    get_filename_component(target ${source_file} NAME_WE)
    add_executable(${target} ${source_file})
    target_link_libraries(${target} PRIVATE ${PROJECT_NAME})
endforeach (source_file ${EXAMPLE_SOURCES})
