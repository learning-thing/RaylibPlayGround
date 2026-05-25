cmake_minimum_required(VERSION 4.2)
project(RaylibPlayGround)

set(CMAKE_CXX_STANDARD 20)

include_directories(include vendor/mujs vendor/GameNetworkingSockets/include/steam)

add_subdirectory(vendor/raylib_git)
add_subdirectory(vendor/GameNetworkingSockets)

add_executable(RaylibPlayGround main.cpp include/jsFuncs.hpp
        include/multiPlayer.hpp)

add_executable(networkTest test/networkTest.cpp)

target_link_libraries(RaylibPlayGround 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/mujs/libmujs.a 
    raylib
    GameNetworkingSockets::static
)
