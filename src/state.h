//
// Created by Doug South on 17/05/2017.
//
#include <SDL2/SDL.h>

#include "controller.h"

#ifndef BREAKOUT_STATE_H_H
#define BREAKOUT_STATE_H_H

typedef struct state state;

struct state {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Joystick *joystick;
    controller_state controller_state;
    SDL_Rect paddleR;
    SDL_Rect ballR;
    int ball_in_play;
};

#endif //BREAKOUT_STATE_H_H
