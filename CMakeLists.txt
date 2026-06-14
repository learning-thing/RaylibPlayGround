cmake_minimum_required(VERSION 3.5)
project(RaylibPlayGround)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 20)
#set(MULTIPLAYER ON)
set(R3D_RAYLIB_VENDORED ON CACHE BOOL "" FORCE)


#add_compile_definitions(GRAPHICS_API_OPENGL_21)
if (MULTIPLAYER)
    add_compile_definitions(multiplayer)
endif ()
#remove_definitions(GRAPHICS_API_OPENGL_33)

include_directories(include vendor/mujs
    vendor/GameNetworkingSockets/include/steam
vendor/r3d/include/)

#add_subdirectory(vendor/raylib_git)
add_subdirectory(vendor/r3d)
if (MULTIPLAYER)
    add_subdirectory(vendor/GameNetworkingSockets)
endif()

add_executable(RaylibPlayGround main.cpp include/jsFuncs.hpp
        include/multiPlayer.hpp
        include/jsShader.cpp
        include/jsShader.hpp
        include/templates.hpp
        include/serial.hpp)

if (MULTIPLAYER)
target_link_libraries(RaylibPlayGround
        PRIVATE
        "-static-libgcc -static-libstdc++"
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/mujs/build/release/libmujs.a
        r3d
        # raylib_static
        GameNetworkingSockets::static
)
else ()
target_link_libraries(RaylibPlayGround
        PRIVATE
        "-static-libgcc -static-libstdc++"
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/mujs/build/release/libmujs.a
        r3d
        # raylib_static
        #GameNetworkingSockets::static
)
endif()
#add_executable(testr3d testr.cpp)
#target_link_libraries(testr3d raylib_static r3d)

add_executable(serialTest testSerial.cpp)
add_executable(generate_templates generate_template.c)
