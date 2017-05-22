#include <SDL2/SDL.h>
#include <time.h>

#include "state.h"
#include "constants.h"
#include "update.h"
#include "render_state.h"

state world_state;

int init() {
    srand((unsigned int) time(NULL));
    int success = 0;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = 0;
    } else {
        world_state.window = SDL_CreateWindow("Breakout",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
        if (world_state.window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = 0;
        } else {
            // TODO: VSYNC???
            world_state.renderer = SDL_CreateRenderer(world_state.window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED);
            if (world_state.renderer == NULL) {
                printf("Renderer could not be create! SDL_Error: %s\n", SDL_GetError());
                success = 0;
            } else if (SDL_NumJoysticks() < 1) {
                // Joystick currently required
                printf("No joystick connected!\n");
                success = 0;
            } else {
                // TODO: Ignore my wheel? ;)
                world_state.joystick = SDL_JoystickOpen(0);
                if (world_state.joystick == NULL) {
                    printf("Unable to open game controller! SDL_Error: %s\n", SDL_GetError());
                    success = 0;
                } else {
                    success = 1;
                }
            }
        }
    }
    return success;
}

void close() {
    SDL_JoystickClose(world_state.joystick);
    SDL_DestroyRenderer(world_state.renderer);
    SDL_DestroyWindow(world_state.window);
    SDL_Quit();
}

int loadMedia() {
    return 1;
}

void event_loop() {
    init_controller_state((controller_state *) &(world_state.controller_state));
    reset_ball(&world_state);
    timer cap;
    init_timer(&cap);
    int quit = 0;
    SDL_Event e;
    const int screen_ticks_per_frame = 1000 / FPS;

    while (!quit) {
        start_timer(&cap);
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else {
                controller_event(e, (controller_state *) &(world_state.controller_state));
            }
        }

        update_state(&world_state);

        SDL_SetRenderDrawColor(world_state.renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(world_state.renderer);

        render_state(&world_state);

        SDL_RenderPresent(world_state.renderer);
        Uint32 frameTicks = getTicks(&cap);
        if (frameTicks < screen_ticks_per_frame) {
            SDL_Delay(screen_ticks_per_frame - frameTicks);
        }
    }
}

int main(int argc, char *args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            event_loop();
        }
    }

    close();
}