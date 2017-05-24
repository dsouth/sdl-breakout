//
// Created by Doug South on 22/05/2017.
//
#include <SDL2/SDL.h>
#include "state.h"
#include "constants.h"


void drawVerticalBoundary(state *s, int x) {
    SDL_Rect rect = {x, 0, x + BORDER_THICKNESS, SCREEN_HEIGHT};
    SDL_RenderFillRect(s->renderer, &rect);
}

void drawHorizontalBoundary(state *s, int y) {
    SDL_Rect rect = {0, y, SCREEN_WIDTH, y + BORDER_THICKNESS};
    SDL_RenderFillRect(s->renderer, &rect);
}

void drawBoundary(state *s) {
    SDL_SetRenderDrawColor(s->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    drawVerticalBoundary(s, 0);
    drawVerticalBoundary(s, SCREEN_WIDTH - BORDER_THICKNESS);
    drawHorizontalBoundary(s, 0);
}

void drawPaddle(state *s) {
    SDL_Rect *p = &((*s).paddleR);
    int paddle = (int) ((((*s).controller_state).left_x_axis + 32768.0) * 1240.0 / 65536.0);
    if (paddle < BORDER_THICKNESS) {
        paddle = BORDER_THICKNESS;
    } else if (paddle > SCREEN_WIDTH - BORDER_THICKNESS - 100) {
        paddle = SCREEN_WIDTH - BORDER_THICKNESS - 100;
    }

    p->x = paddle;
    p->y = SCREEN_HEIGHT - BORDER_THICKNESS;
    p->w = 99;
    p->h = PADDLE_THICKNESS;
    SDL_RenderFillRect(s->renderer, p);
    SDL_SetRenderDrawColor(s->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderDrawLine(s->renderer, paddle - 1, SCREEN_HEIGHT - BORDER_THICKNESS,
                       paddle - 1, SCREEN_HEIGHT);
    SDL_RenderDrawLine(s->renderer, paddle + 100, SCREEN_HEIGHT - BORDER_THICKNESS,
                       paddle + 100, SCREEN_HEIGHT);
}

void drawBall(state *s) {
    SDL_SetRenderDrawColor(s->renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(s->renderer, &s->ball.ballR);
}

void drawBricks(state *s) {
    SDL_SetRenderDrawColor(s->renderer, 0x00, 0xFF, 0xFF, 0xFF);
    for (Uint32 i = 0; i < s->brick_count; i ++) {
        brick *b = &s->bricks[i];
        if (b->showing) {
            SDL_RenderFillRect(s->renderer, &b->rect);
        }

    }

}

void render_state(state *s) {
    drawBoundary(s);
    drawPaddle(s);
    drawBall(s);
    drawBricks(s);
}


