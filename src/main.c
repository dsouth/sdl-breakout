#include <SDL2/SDL.h>
#include <time.h>

#include "state.h"
#include "constants.h"
#include "update.h"

state world;

int init() {
    srand((unsigned int) time(NULL));
    int success = 0;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = 0;
    } else {
        world.window = SDL_CreateWindow("Breakout",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
        if (world.window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = 0;
        } else {
            // TODO: VSYNC???
            world.renderer = SDL_CreateRenderer(world.window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED);
            if (world.renderer == NULL) {
                printf("Renderer could not be create! SDL_Error: %s\n", SDL_GetError());
                success = 0;
            } else if (SDL_NumJoysticks() < 1) {
                // Joystick currently required
                printf("No joystick connected!\n");
                success = 0;
            } else {
                // TODO: Ignore my wheel? ;)
                world.joystick = SDL_JoystickOpen(0);
                if (world.joystick == NULL) {
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
    SDL_JoystickClose(world.joystick);
    SDL_DestroyRenderer(world.renderer);
    SDL_DestroyWindow(world.window);
    SDL_Quit();
}

int loadMedia() {
    return 1;
}

void drawVerticalBoundary(int x) {
    SDL_Rect rect = {x, 0, x + BORDER_THICKNESS, SCREEN_HEIGHT};
    SDL_RenderFillRect(world.renderer, &rect);
}

void drawHorizontalBoundary(int y) {
    SDL_Rect rect = {0, y, SCREEN_WIDTH, y + BORDER_THICKNESS};
    SDL_RenderFillRect(world.renderer, &rect);
}

void drawBoundary() {
    SDL_SetRenderDrawColor(world.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    drawVerticalBoundary(0);
    drawVerticalBoundary(SCREEN_WIDTH - BORDER_THICKNESS);
    drawHorizontalBoundary(0);
}

void drawPaddle(state *s) {
    SDL_Rect *p = &((*s).paddleR);
    int paddle = (int) ((((*s).controller_state).left_x_axis + 32768.0) * 1240.0 / 65536.0);
    if (paddle < BORDER_THICKNESS) {
        paddle = BORDER_THICKNESS;
    } else if (paddle > SCREEN_WIDTH - BORDER_THICKNESS - 100) {
        paddle = SCREEN_WIDTH - BORDER_THICKNESS - 100;
    }

    p->x = paddle;
    p->y = SCREEN_HEIGHT - BORDER_THICKNESS;
    p->w = 99;
    p->h = PADDLE_THICKNESS;
    SDL_RenderFillRect(world.renderer, p);
    SDL_SetRenderDrawColor(world.renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderDrawLine(world.renderer, paddle - 1, SCREEN_HEIGHT - BORDER_THICKNESS,
                       paddle - 1, SCREEN_HEIGHT);
    SDL_RenderDrawLine(world.renderer, paddle + 100, SCREEN_HEIGHT - BORDER_THICKNESS,
                       paddle + 100, SCREEN_HEIGHT);
}

void drawBall(state *s) {
    SDL_SetRenderDrawColor(world.renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(world.renderer, &s->ball.ballR);
}

void renderWorld(state *s) {
    drawBoundary();
    drawPaddle(s);
    drawBall(s);
}

void event_loop() {
    init_controller_state((controller_state *) &(world.controller_state));
    reset_ball(&world);
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
                controller_event(e, (controller_state *) &(world.controller_state));
            }
        }

        update_state(&world);

        SDL_SetRenderDrawColor(world.renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(world.renderer);

        renderWorld(&world);

        SDL_RenderPresent(world.renderer);
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