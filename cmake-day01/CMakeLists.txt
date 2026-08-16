cmake_minimum_required(VERSION 3.16)
project(cmake_day01 CXX)

add_executable(app src/main.cpp src/math_utils.cpp)
target_include_directories(app PUBLIC include)
