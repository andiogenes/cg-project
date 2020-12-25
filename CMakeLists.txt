cmake_minimum_required(VERSION 3.17)
project(minesruft)

find_package(raylib 2.5.0 REQUIRED PATHS raylib)

set(CMAKE_CXX_STANDARD 14)

add_executable(minesruft main.cpp)

target_link_libraries(minesruft raylib)