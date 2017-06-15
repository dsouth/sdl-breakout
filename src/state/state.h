//
// Created by Doug South on 17/05/2017.
//

#include "../controller.h"
#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "sound.h"
#include "config.h"

#ifndef BREAKOUT_STATE_H_H
#define BREAKOUT_STATE_H_H
typedef struct state state;

struct state {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Joystick *joystick;
    controller_state controller_state;
    paddle paddle;
    ball ball;
    brick *bricks;
    Uint32 brick_count;
    sound sound;
    config config;
};

#endif //BREAKOUT_STATE_H_H
