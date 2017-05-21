//
// Created by Doug South on 21/05/2017.
//

#include "timer.h"

void init_timer(timer *t) {
    t->startTicks = 0;
    t->pausedTicks = 0;
    t->paused = 0;
    t->started = 0;
}

void start_timer(timer *t) {
    t->started = 1;
    t->paused = 0;
    t->startTicks = SDL_GetTicks();
    t->pausedTicks = 0;
}

/*
void stop_timer(timer *t) {
    t->started = 0;
    t->paused = 0;
    t->startTicks = 0;
    t->pausedTicks = 0;
}

void pause_timer(timer *t) {
    if (t->started && !t->paused) {
        t->paused = 1;
        t->pausedTicks = SDL_GetTicks() - t->startTicks;
        t->startTicks = 0;
    }
}

void unpause_timer(timer *t) {
    if (t->started && t->paused) {
        t->paused = 0;
        t->startTicks = SDL_GetTicks() - t->pausedTicks;
        t->pausedTicks = 0;
    }
}
 */

Uint32 getTicks(timer *t) {
    Uint32 time = 0;
    if (t->started) {
        if (t->paused) {
            time = t->pausedTicks;
        } else {
            time = SDL_GetTicks() - t->startTicks;
        }
    }
    return time;
}

