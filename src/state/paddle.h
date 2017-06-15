//
// Created by Doug South on 15/6/17.
//

#ifndef BREAKOUT_PADDLE_H
#define BREAKOUT_PADDLE_H

#include <SDL_rect.h>

typedef struct paddle paddle;

struct paddle {
    SDL_Rect paddleR;
    double x;
};

#endif //BREAKOUT_PADDLE_H
