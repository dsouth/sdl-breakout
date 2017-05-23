cmake_minimum_required(VERSION 3.7)
project(breakout)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES src/main.c
        src/controller.c src/update.c src/constants.h src/update.h src/render_state.c src/render_state.h)
include_directories(/usr/local/include/)
link_directories(/usr/local/lib/)
add_executable(breakout ${SOURCE_FILES})
target_link_libraries(breakout SDL2)