cmake_minimum_required(VERSION 4.2)
project(RaylibPlayGround)

set(CMAKE_CXX_STANDARD 20)

include_directories(include vendor/mujs)

add_subdirectory(vendor/raylib_git)

add_executable(RaylibPlayGround main.cpp include/jsFuncs.hpp)

target_link_libraries(RaylibPlayGround 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/mujs/libmujs.a 
    raylib
)
