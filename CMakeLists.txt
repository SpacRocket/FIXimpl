cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=* --")
set(CMAKE_CXX_STANDARD 20)
project(bat)
add_subdirectory(bfxclient)

add_executable(bat main.cpp)