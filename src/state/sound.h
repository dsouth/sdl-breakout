//
// Created by Doug South on 15/6/17.
//

#ifndef BREAKOUT_SOUND_H
#define BREAKOUT_SOUND_H

#include <SDL2_mixer/SDL_mixer.h>

typedef struct sound sound;

struct sound {
    Mix_Chunk *beep;
//    Mix_Chunk *peeeeeep;
    Mix_Chunk *plop;
};

#endif //BREAKOUT_SOUND_H
