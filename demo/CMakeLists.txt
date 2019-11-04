
include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(sts_demo sts_demo.c)
target_link_libraries(sts_demo cos_sts)

