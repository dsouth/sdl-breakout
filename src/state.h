//
// Created by Doug South on 17/05/2017.
//
#include <SDL2/SDL.h>

#include "controller.h"
#include "timer.h"

#ifndef BREAKOUT_STATE_H_H
#define BREAKOUT_STATE_H_H

typedef struct ball ball;

struct ball {
    SDL_Rect ballR;
    int ball_in_play;
    double x;
    double y;
    double dx;
    double dy;
    double speed;
    int missed;
};

typedef struct state state;

struct state {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Joystick *joystick;
    controller_state controller_state;
    SDL_Rect paddleR;
    ball ball;
};

#endif //BREAKOUT_STATE_H_H
