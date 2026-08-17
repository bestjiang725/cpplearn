cmake_minimum_required(VERSION 3.16)
project(cmake_day01 CXX)

add_library(math_utils STATIC src/math_utils.cpp)
target_include_directories(math_utils PUBLIC include)

add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE math_utils)

add_executable(test src/test.cpp)
target_link_libraries(test PRIVATE math_utils)
