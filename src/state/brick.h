//
// Created by Doug South on 15/6/17.
//

#ifndef BREAKOUT_BRICK_H
#define BREAKOUT_BRICK_H

#include <SDL2/SDL.h>

typedef struct brick brick;

struct brick {
    SDL_Rect rect;
    SDL_Color color;
    int showing;
};


#endif //BREAKOUT_BRICK_H
