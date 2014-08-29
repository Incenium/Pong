#ifndef BALL_H
#define BALL_H

#include <string>
#include <sstream>
#include "SDL2/SDL.h"
#include "paddle.h"

class Ball {
    SDL_Texture *image = NULL;
    SDL_Rect collisionBox;

    float xVel = 200.0f;
    float yVel = 200.0f;

  public:
    void init(int x, int y, SDL_Texture *image);
    void update(int delta, Paddle *paddle1, Paddle *paddle2);
    void draw(SDL_Renderer *renderer);
    void free();
};
#endif