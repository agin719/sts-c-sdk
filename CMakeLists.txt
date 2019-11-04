project(sts)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS " -g -ggdb -O0 -Werror -fpic -fPIC")
set(CMAKE_CXX_FLAGS " -g -ggdb -O0 -Werror -fpic -fPIC")

set(CMAKE_C_FLAGS_DEBUG " -g -ggdb -O0 -Werror -fpic -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG " -g -ggdb -O0 -Werror -fpic -fPIC")

set(CMAKE_C_FLAGS_RELEASE " -O3 -Werror -fpic -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE " -O3 -Werror -fpic -fPIC")

add_subdirectory(src)
add_subdirectory(demo)
