#include <SDL2/SDL.h>

#include "controller.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const int BORDER_THICKNESS = 20;

int init();

int loadMedia();

void close();

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

SDL_Joystick *gController;

int init() {
    int success = 0;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = 0;
    } else {
        gWindow = SDL_CreateWindow("Breakout",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SCREEN_WIDTH,
                                   SCREEN_HEIGHT,
                                   SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = 0;
        } else {
            // TODO: VSYNC???
            gRenderer = SDL_CreateRenderer(gWindow,
                                           -1,
                                           SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                printf("Renderer could not be create! SDL_Error: %s\n", SDL_GetError());
                success = 0;
            } else if (SDL_NumJoysticks() < 1) {
                // Joystick currently required
                printf("No joystick connected!\n");
                success = 0;
            } else {
                // TODO: Ignore my wheel? ;)
                gController = SDL_JoystickOpen(0);
                if (gController == NULL) {
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
    SDL_JoystickClose(gController);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

int loadMedia() {
    return 1;
}

void drawVerticalBoundary(int x) {
    SDL_Rect rect = {x, 0, x + BORDER_THICKNESS, SCREEN_HEIGHT};
    SDL_RenderFillRect(gRenderer, &rect);
}

void drawHorizontalBoundary(int y) {
    SDL_Rect rect = {0, y, SCREEN_WIDTH, y + BORDER_THICKNESS};
    SDL_RenderFillRect(gRenderer, &rect);
}

void event_loop() {
    int quit = 0;
    SDL_Event e;
    controller_state s;
    init_controller_state(&s);

    while (!quit) {
        while(SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else {
                controller_event(e, &s);
            }
        }
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(gRenderer);

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        drawVerticalBoundary(0);
        drawVerticalBoundary(SCREEN_WIDTH - BORDER_THICKNESS);
        drawHorizontalBoundary(0);

        SDL_RenderPresent(gRenderer);
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