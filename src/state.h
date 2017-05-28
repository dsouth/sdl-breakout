//
// Created by Doug South on 17/05/2017.
//
#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>

#include "controller.h"

#ifndef BREAKOUT_STATE_H_H
#define BREAKOUT_STATE_H_H

typedef struct ball ball;

struct ball {
    SDL_Rect ballR;
    SDL_Color color;
    int ball_in_play;
    double x;
    double y;
    double dx;
    double dy;
    double speed;
    int missed;
};

typedef struct brick brick;

struct brick {
    SDL_Rect rect;
    SDL_Color color;
    int showing;
};

typedef struct sound sound;

struct sound {
    Mix_Chunk *beep;
    Mix_Chunk *peeeeeep;
    Mix_Chunk *plop;
};

typedef struct state state;

struct state {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Joystick *joystick;
    controller_state controller_state;
    SDL_Rect paddleR;
    ball ball;
    brick *bricks;
    Uint32 brick_count;
    sound sound;
};

#endif //BREAKOUT_STATE_H_H
