#include <SDL2/SDL.h>

#include "state.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const int BORDER_THICKNESS = 20;

state world;

int init() {
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
    drawVerticalBoundary(0);
    drawVerticalBoundary(SCREEN_WIDTH - BORDER_THICKNESS);
    drawHorizontalBoundary(0);
}

void drawPaddle(state *s) {
    int paddle = (int) ((((*s).controller_state).left_x_axis + 32768.0) * 1240.0 / 65536.0);
    if (paddle < BORDER_THICKNESS) {
        paddle = BORDER_THICKNESS;
    } else if (paddle > SCREEN_WIDTH - BORDER_THICKNESS - 100) {
        paddle = SCREEN_WIDTH - BORDER_THICKNESS - 100;
    }

    (*s).paddleR.x = paddle;
    (*s).paddleR.y = SCREEN_HEIGHT - BORDER_THICKNESS;
    (*s).paddleR.w = 99;
    (*s).paddleR.h = BORDER_THICKNESS;
    SDL_RenderFillRect(world.renderer, &((*s).paddleR));
    SDL_SetRenderDrawColor(world.renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderDrawLine(world.renderer, paddle - 1, SCREEN_HEIGHT - BORDER_THICKNESS,
                       paddle - 1, SCREEN_HEIGHT);
    SDL_RenderDrawLine(world.renderer, paddle + 100, SCREEN_HEIGHT - BORDER_THICKNESS,
                       paddle + 100, SCREEN_HEIGHT);
}

void drawBall(state *s) {
    SDL_SetRenderDrawColor(world.renderer, 0xFF, 0x00, 0x00, 0xFF);
    if ((*s).ball_in_play) {
        // move the ball and do stuff ;)
    } else {
        (*s).ballR.x = (*s).paddleR.x + 50 - BORDER_THICKNESS/2;
    }
    SDL_RenderFillRect(world.renderer, &(world.ballR));
}

void event_loop() {
    int quit = 0;
    SDL_Event e;
    init_controller_state(&(world.controller_state));
    world.ballR.x = SCREEN_WIDTH / 2 + BORDER_THICKNESS;
    world.ballR.y = SCREEN_HEIGHT - BORDER_THICKNESS - BORDER_THICKNESS - 1;
    world.ballR.w = BORDER_THICKNESS;
    world.ballR.h = BORDER_THICKNESS;
    world.ball_in_play = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else {
                controller_event(e, &(world.controller_state));
            }
        }
        SDL_SetRenderDrawColor(world.renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(world.renderer);

        SDL_SetRenderDrawColor(world.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        drawBoundary();
        drawPaddle(&(world));

        drawBall(&world);

        SDL_RenderPresent(world.renderer);
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