//
// Created by Doug South on 15/6/17.
//

#ifndef BREAKOUT_BALL_H
#define BREAKOUT_BALL_H

#include <SDL_rect.h>

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
#endif //BREAKOUT_BALL_H
