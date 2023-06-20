cmake_minimum_required(VERSION 3.1)

add_definitions(-std=c++11)

project(stl_list C CXX)

set(SRC_LIST main.cpp)

add_executable(stl_list ${SRC_LIST})