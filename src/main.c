#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <time.h>

#include "state.h"
#include "constants.h"
#include "update.h"
#include "render_state.h"

state world_state;

int init() {
    srand((unsigned int) time(NULL));
    int success = 0;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) < 0) {
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
            world_state.renderer = SDL_CreateRenderer(world_state.window,
                                                      -1,
                                                      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
                    if (Mix_OpenAudio(44100, AUDIO_S16, 2, 2048)  != 0) {
                        printf("SDL_Mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                        success = 0;
                    } else {
                        success = 1;
                    }
                }
            }
        }
    }
    return success;
}

void close() {
    Mix_Quit();
    SDL_JoystickClose(world_state.joystick);
    SDL_DestroyRenderer(world_state.renderer);
    SDL_DestroyWindow(world_state.window);
    SDL_Quit();
}

int loadMedia(state *s) {
    s->sound.beep = Mix_LoadWAV("res/beep.ogg");
    if (s->sound.beep == NULL) {
        printf("Failed to load beep sound! SDL_Error: %s\n", SDL_GetError());
    }
//    s->sound.peeeeep = Mix_LoadWAV("res/peeeeeep.ogg");
//    if (s->sound.peeeeeep == NULL) {
//        printf("Failed to load peeeeeep sound! SDL_Error: %s\n", SDL_GetError());
//    }
    s->sound.plop = Mix_LoadWAV("res/plop.ogg");
    if (s->sound.plop == NULL) {
        printf("Failed to load plop sound! SDL_Error: %s\n", SDL_GetError());
    }
    return 1;
}

// assumes 60 FPS
Uint8 expected_interval[] = {17, 17, 16};
Uint8 expected_index = 0;
Uint32 expected_time;

Uint32 get_next_expected_time_delta() {
    Uint32 d = expected_interval[expected_index];
    expected_index++;
    if (expected_index > 2) {
        expected_index = 0;
    }
    return d;
}

void init_brick(state *s) {
    Uint32 rows = 5;
    Uint32 columns = 15;
    Uint32 count = rows * columns;
    s->bricks = malloc(sizeof(brick) * count);
    s->brick_count = count;
    const Uint16 brick_width = 80;
    const Uint16 brick_height = 40;
    const Uint16 width_gap = 2;
    const Uint16 height_gap = 5;
    const Uint16 top_y = 200;
    const Uint16 top_x = 27;
    for (Uint16 r = 0; r < rows; r++) {
        for (Uint16 i = 0; i < columns; i++) {
            SDL_Rect *rect = &s->bricks[i + r * columns].rect;
            rect->x = top_x + i * (brick_width + width_gap);
            rect->y = top_y + r * (brick_height + height_gap);
            rect->w = brick_width;
            rect->h = brick_height;
            s->bricks[i + r * columns].showing = 1;
            s->bricks[i + r * columns].color.r = (Uint8) (rand() % 256);
            s->bricks[i + r * columns].color.g = (Uint8) (rand() % 256);
            s->bricks[i + r * columns].color.b = (Uint8) (rand() % 256);
            s->bricks[i + r * columns].color.a = 0xFF;
        }
    }
}

void event_loop() {
    init_controller_state((controller_state *) &(world_state.controller_state));
    reset_ball(&world_state);
    init_brick(&world_state);
    SDL_SetRenderDrawBlendMode(world_state.renderer, SDL_BLENDMODE_BLEND);
    int quit = 0;
    SDL_Event e;

    expected_time = get_next_expected_time_delta() + SDL_GetTicks();

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else {
                controller_event(e, (controller_state *) &(world_state.controller_state));
            }
        }

        update_state(&world_state);
        // don't render if behind
        if (SDL_GetTicks() < expected_time) {
            SDL_SetRenderDrawColor(world_state.renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(world_state.renderer);

            render_state(&world_state);

            SDL_RenderPresent(world_state.renderer);
            Uint32 ticks = SDL_GetTicks();
            // delay if we are ahead
            if (ticks < expected_time) {
                SDL_Delay(expected_time - ticks);
            }
        }
        expected_time += get_next_expected_time_delta();
    }
}

int main(int argc, char *args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        if (!loadMedia(&world_state)) {
            printf("Failed to load media!\n");
        } else {
            event_loop();
        }
    }

    close();
}