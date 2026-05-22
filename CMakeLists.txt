cmake_minimum_required(VERSION 4.2)
project(RaylibPlayGround)

set(CMAKE_CXX_STANDARD 20)
#SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}")

link_directories(vendor/mujs/build/release)

include_directories(include)

add_subdirectory(vendor/raylib_git)
add_executable(RaylibPlayGround main.cpp include/jsFuncs.hpp)

target_link_libraries(RaylibPlayGround  libmujs.a raylib)
