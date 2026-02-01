cmake_minimum_required(VERSION 3.26)
project(physics_sim C)

set(CMAKE_C_STANDARD 11)

set(RAYLIB_INCLUDE_DIR "C:/raylib/raylib/src")
set(RAYLIB_LIB_DIR "C:/raylib/raylib/src") 

include_directories(${RAYLIB_INCLUDE_DIR})
link_directories(${RAYLIB_LIB_DIR})

add_executable(physics_sim main.c
        game.c
        game.h
        menu.c
        menu.h)

target_link_libraries(physics_sim
        raylib
        opengl32
        gdi32
        winmm
)
