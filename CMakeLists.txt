cmake_minimum_required(VERSION 3.7)
project(breakout)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/build/cmake")

find_package(PkgConfig)
pkg_search_module(SDL2 sdl2)
pkg_search_module(SDL2_mixer sdl2_mixer)

if(NOT "${SDL2_FOUND}")
    message("looking for SDL2")
    find_package(SDL2 REQUIRED)
endif()
if(NOT "${SDL2_MIXER_FOUND}")
    message("looking for SDL2_mixer")
    find_package(SDL2_mixer REQUIRED)
endif()

set(SOURCE_FILES src/main.c
        src/controller.c src/update.c src/constants.h src/update.h src/render_state.c src/render_state.h src/state/ball.h src/state/paddle.h src/state/brick.h src/state/sound.h src/state/config.h)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})
add_executable(breakout ${SOURCE_FILES})
target_link_libraries(breakout ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARY})