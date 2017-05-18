#include <SDL2/SDL.h>
#include <time.h>

#include "state.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const int BORDER_THICKNESS = 20;
const int BALL_THICKNESS = BORDER_THICKNESS;
const int PADDLE_THICKNESS = BORDER_THICKNESS;

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

void reset_ball(state *s) {
    ball *b = (ball *) &((*s).ball);
    b->ball_in_play = 0;
    b->speed = 1.0;
    b->missed = 0;
    SDL_Rect *ballR = &(*s).ball.ballR;
    ballR->x = SCREEN_WIDTH / 2 + BALL_THICKNESS;
    ballR->y = SCREEN_HEIGHT - BORDER_THICKNESS - BALL_THICKNESS - 1;
    ballR->w = BALL_THICKNESS;
    ballR->h = BALL_THICKNESS;
}

void move_ball(state *s) {
    ball *b = (ball *) &((*s).ball);
    b->x += b->dx * b->speed;
    b->y += b->dy * b->speed;
    b->ballR.x = (int)b->x;
    b->ballR.y = (int)b->y;
    if (b->ballR.y < BORDER_THICKNESS) {
        b->dy = -b->dy;
        b->ballR.y = BORDER_THICKNESS + (BORDER_THICKNESS - b->ballR.y);
    } else if (b->ballR.y > SCREEN_HEIGHT - PADDLE_THICKNESS - BALL_THICKNESS) {
        if (!b->missed) {
            // do we hit the ball
            SDL_Rect *p = &((*s).paddleR);
            // paddle 'hit' ball
            if (b->x > p->x - BALL_THICKNESS && b->x < p->x + p->w + 1) {
                // find a new angle here please?
                b->dy = -b->dy;
                b->speed += 0.1;
                b->ballR.y = SCREEN_HEIGHT - PADDLE_THICKNESS - BALL_THICKNESS
                             + (b->ballR.y - SCREEN_HEIGHT + PADDLE_THICKNESS + BALL_THICKNESS);
            } else {
                b->missed = 1;
            }
        } else {
            if (b->ballR.y > SCREEN_HEIGHT) {
                reset_ball(s);
            }
        }
    }
    if (b->ballR.x < BORDER_THICKNESS) {
        b->dx = -b->dx;
        b->ballR.x = BORDER_THICKNESS + (BORDER_THICKNESS - b->ballR.x);
    } else if (b->ballR.x > SCREEN_WIDTH - BORDER_THICKNESS - BALL_THICKNESS) {
        b->dx = -b->dx;
        b->ballR.x = SCREEN_WIDTH - BORDER_THICKNESS - BALL_THICKNESS
                     - (b->ballR.x - SCREEN_WIDTH + BORDER_THICKNESS + BALL_THICKNESS);
    }
}

void updateWorld(state *s) {
    ball *b = (ball *) &((*s).ball);
    if (b->ball_in_play) {
        move_ball(s);
    } else {
        if ((*s).controller_state.button_a == SDL_PRESSED) {
            b->ball_in_play = 1;
            int i = rand() % 60 + 60;
            double angle = i * (M_PI / 180);
            b->dx = cos(angle);
            b->dy = -sin(angle);
            move_ball(s);
        } else {
            b->ballR.x = (*s).paddleR.x + 50 - BALL_THICKNESS / 2;
            b->x = b->ballR.x;
            b->y = b->ballR.y;
        }
    }
}

void event_loop() {
    reset_ball(&world);
    int quit = 0;
    SDL_Event e;
    init_controller_state((controller_state *) &(world.controller_state));

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else {
                controller_event(e, (controller_state *) &(world.controller_state));
            }
        }
        updateWorld(&world);

        SDL_SetRenderDrawColor(world.renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(world.renderer);

        renderWorld(&world);

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