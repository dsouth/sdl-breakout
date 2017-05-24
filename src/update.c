//
// Created by Doug South on 22/05/2017.
//

#include "state.h"
#include "constants.h"

void reset_ball(state *s) {
    ball *b = (ball *) &((*s).ball);
    b->ball_in_play = 0;
    b->speed = 480.0 / FPS;
    b->missed = 0;
    SDL_Rect *ballR = &(*s).ball.ballR;
    ballR->x = SCREEN_WIDTH / 2 + BALL_THICKNESS;
    ballR->y = SCREEN_HEIGHT - BORDER_THICKNESS - BALL_THICKNESS - 1;
    ballR->w = BALL_THICKNESS;
    ballR->h = BALL_THICKNESS;
}

void move_ball(state *s) {
    SDL_Rect result;
    ball *b = (ball *) &((*s).ball);
    b->x += b->dx * b->speed;
    b->y += b->dy * b->speed;
    b->ballR.x = (int)b->x;
    b->ballR.y = (int)b->y;
    // if hit top boundary
    if (b->ballR.y < BORDER_THICKNESS) {
        b->dy = -b->dy;
        b->ballR.y = BORDER_THICKNESS + (BORDER_THICKNESS - b->ballR.y);

        // if ball at or beyond paddle and going down
    } else if (b->ballR.y > SCREEN_HEIGHT - PADDLE_THICKNESS - BALL_THICKNESS && b->dy > 0) {
        // if the paddle hasn't yet missed the ball
        if (!b->missed) {
            // do we hit the ball
            SDL_Rect *p = &((*s).paddleR);
            // paddle 'hit' ball
            if (b->x > p->x - BALL_THICKNESS && b->x < p->x + p->w + 1) {
                double current_ball_angle = acos(b->dx);
                // the hit width for the paddle
                double hit_width = p->w + b->ballR.w;
                double middle = (hit_width / 2);
                double denominator = p->x - b->ballR.x - b->ballR.w + middle;
                double v = denominator / middle;
                double angle_adjustment = v * 1.0472;
                current_ball_angle += angle_adjustment;
                if (current_ball_angle < MIN_RADIAN) {
                    current_ball_angle = MIN_RADIAN;
                } else if (current_ball_angle > M_PI - MIN_RADIAN) {
                    current_ball_angle = M_PI - MIN_RADIAN;
                }
                b->dx = cos(current_ball_angle);
                b->dy = -sin(current_ball_angle);
                b->speed += 0.1;
                b->ballR.y = SCREEN_HEIGHT - PADDLE_THICKNESS - BALL_THICKNESS
                             + (b->ballR.y - SCREEN_HEIGHT + PADDLE_THICKNESS + BALL_THICKNESS);
            } else {
                b->missed = 1;
            }
        } else {
            if (b->ballR.y > SCREEN_HEIGHT) {
                reset_ball(s);
            }
        }
        // if ball hit left border
    } else if (b->ballR.x < BORDER_THICKNESS) {
        b->dx = -b->dx;
        b->ballR.x = BORDER_THICKNESS + (BORDER_THICKNESS - b->ballR.x);
        // if ball hit right border
    } else if (b->ballR.x > SCREEN_WIDTH - BORDER_THICKNESS - BALL_THICKNESS) {
        b->dx = -b->dx;
        b->ballR.x = SCREEN_WIDTH - BORDER_THICKNESS - BALL_THICKNESS
                     - (b->ballR.x - SCREEN_WIDTH + BORDER_THICKNESS + BALL_THICKNESS);
    } else {
        int bounced = 0;
        for (Uint32 i = 0; i < s->brick_count; i++) {
            if (s->bricks[i].showing && SDL_IntersectRect(&b->ballR, &s->bricks[i].rect, &result)) {
                s->bricks[i].showing = 0;
                if (!bounced) {
                    b->dy = -b->dy;
                    // bounce the ball
                }
                bounced = 1;
            }
        }
    }
}

void update_state(state *s) {
    ball *b = (ball *) &((*s).ball);
    if (b->ball_in_play) {
        move_ball(s);
    } else {
        if ((*s).controller_state.button_a == SDL_PRESSED) {
            b->ball_in_play = 1;
            int i = rand() % 60 + 60;
            double angle = i * (M_PI / 180);
            b->dx = cos(angle);
            b->dy = -sin(angle);
            move_ball(s);
        } else {
            b->ballR.x = (*s).paddleR.x + 50 - BALL_THICKNESS / 2;
            b->x = b->ballR.x;
            b->y = b->ballR.y;
        }
    }

}

