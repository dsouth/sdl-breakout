//
// Created by Doug South on 21/05/2017.
//

#include <SDL2/SDL.h>

#ifndef BREAKOUT_TIME_H
#define BREAKOUT_TIME_H

typedef struct timer timer;

struct timer {
    Uint32 startTicks;
    Uint32 pausedTicks;
    int started;
    int paused;
};

void init_timer(timer *t);

void start_timer(timer *t);
//void stop_timer(timer *t);
//void pause_timer(timer *t);
//void unpause_timer(timer *t);
Uint32 getTicks(timer *t);

#endif //BREAKOUT_TIME_H
