cmake_minimum_required(VERSION 3.16)
project(cp-lib CXX)


add_library(num STATIC src/num.cpp)
target_include_directories(num PUBLIC include)

add_executable(demo src/demo.cpp)
target_link_libraries(demo PRIVATE num)