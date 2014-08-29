#ifndef PADDLE_H
#define PADDLE_H

#include "SDL2/SDL.h"

class Paddle {
    //SDL_Rect collisionBox;
    SDL_Texture *image = NULL;

    float velocity = 0.f; /* velocity in pixels */

  public:
    void init(int x, SDL_Texture *image);
    void free();
    void handleInput(SDL_Event event);
    void update(int delta);
    void draw(SDL_Renderer *renderer);
    
    SDL_Rect collisionBox;
};
#endif