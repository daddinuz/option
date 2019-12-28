add_library(features ${CMAKE_CURRENT_LIST_DIR}/features.h ${CMAKE_CURRENT_LIST_DIR}/features.c)
target_link_libraries(features PRIVATE panic traits-unit)

add_executable(describe ${CMAKE_CURRENT_LIST_DIR}/describe.c)
target_link_libraries(describe PRIVATE features)

add_test(describe describe)
enable_testing()
