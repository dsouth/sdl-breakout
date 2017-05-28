//
// Created by Doug South on 22/05/2017.
//
#include <SDL2/SDL.h>
#include "state.h"
#include "constants.h"

void setRenderColor(SDL_Renderer *r, SDL_Color *c) {
    if (SDL_SetRenderDrawColor(r, c->r, c->g, c->b, c->a) == -1) {
        printf("Error setting color: %d, %d, %d, %d! SDL_Error: %s\n", c->r, c->b, c->b, c->a, SDL_GetError());
    }
}


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

void tint(SDL_Color c, SDL_Color *l) {
    double factor = 0.5;
    l->r = (Uint8) (c.r + (255 - c.r) * factor);
    l->g = (Uint8) (c.g + (255 - c.g) * factor);
    l->b = (Uint8) (c.b + (255 - c.b) * factor);
    l->a = c.a;
}

void shade(SDL_Color c, SDL_Color *l) {
    double factor = 0.5;
    l->r = (Uint8) (c.r * (1 - factor));
    l->g = (Uint8) (c.g * (1 - factor));
    l->b = (Uint8) (c.b * (1 - factor));
    l->a = c.a;
}

void highlight_rect(state *s, SDL_Color *c, SDL_Rect *r) {
    SDL_Color highlight;
    tint(*c, &highlight);
    setRenderColor(s->renderer, &highlight);
    SDL_RenderDrawLine(s->renderer, r->x, r->y, r->x + r->w - 1, r->y);
    SDL_RenderDrawLine(s->renderer, r->x, r->y, r->x, r->y + r->h - 1);
    shade(*c, &highlight);
    setRenderColor(s->renderer, &highlight);
    SDL_RenderDrawLine(s->renderer, r->x + r->w - 1, r->y,
                       r->x + r->w - 1, r->y + r->h - 1);
    SDL_RenderDrawLine(s->renderer, r->x, r->y + r->h - 1,
                       r->x + r->w - 1, r->y + r->h - 1);

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
    SDL_Color c;
    c.r = c.g = c.b = c.a = 0xFF;
    setRenderColor(s->renderer, &c);
    SDL_RenderFillRect(s->renderer, p);
    highlight_rect(s, &c, p);
    SDL_SetRenderDrawColor(s->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderDrawLine(s->renderer, paddle - 1, SCREEN_HEIGHT - BORDER_THICKNESS,
                       paddle - 1, SCREEN_HEIGHT);
    SDL_RenderDrawLine(s->renderer, paddle + 100, SCREEN_HEIGHT - BORDER_THICKNESS,
                       paddle + 100, SCREEN_HEIGHT);
}


void drawBall(state *s) {
    setRenderColor(s->renderer, &s->ball.color);
    SDL_RenderFillRect(s->renderer, &s->ball.ballR);
    highlight_rect(s, &s->ball.color, &s->ball.ballR);
}


void drawBricks(state *s) {
    for (Uint32 i = 0; i < s->brick_count; i ++) {
        brick *brick = &s->bricks[i];
        if (brick->showing) {
            setRenderColor(s->renderer, &brick->color);
            SDL_RenderFillRect(s->renderer, &brick->rect);
            highlight_rect(s, &brick->color, &brick->rect);
        }

    }

}

void render_state(state *s) {
    drawBoundary(s);
    drawPaddle(s);
    drawBall(s);
    drawBricks(s);
}


