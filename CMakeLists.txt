cmake_minimum_required(VERSION 3.5)
project(option)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcast-align -Wbad-function-cast")
endif ()

#####
# Working directories
###
set(EXAMPLE_PATH ${PROJECT_SOURCE_DIR}/examples)
set(SOURCE_PATH ${PROJECT_SOURCE_DIR}/sources)
include_directories(${SOURCE_PATH})

#####
# Archive
###
file(GLOB HEADER_FILES ${SOURCE_PATH}/*.h)
file(GLOB SOURCE_FILES ${SOURCE_PATH}/*.c)
add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

#####
# Examples
###
file(GLOB EXAMPLE_SOURCES ${EXAMPLE_PATH}/example*.c)
foreach (source_file ${EXAMPLE_SOURCES})
    get_filename_component(target ${source_file} NAME_WE)
    add_executable(${target} ${source_file})
    target_link_libraries(${target} PRIVATE ${PROJECT_NAME})
endforeach (source_file ${EXAMPLE_SOURCES})
