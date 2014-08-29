#include "paddle.h"

void Paddle::init(int x, SDL_Texture *iimage)
{
    collisionBox.x = x;
    collisionBox.y = 480 / 2;
    collisionBox.w = 24;
    collisionBox.h = 94;

    image = &*iimage;

    if (image == NULL){
        printf("Image is null, major bummer man\n");
    }
}

void Paddle::handleInput(SDL_Event event)
{
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0){
        switch (event.key.keysym.sym){
            case SDLK_UP: velocity = -200.0f; break;
            case SDLK_DOWN: velocity = 200.0f; break;
        }
    }

    else if (event.type == SDL_KEYUP && event.key.repeat == 0){
        switch (event.key.keysym.sym){
            case SDLK_UP: velocity = 0.0f; break;
            case SDLK_DOWN: velocity = 0.0f; break;
        }
    }
}

void Paddle::update(int delta)
{
    collisionBox.y += velocity * (delta / 1000.0f);

    if (collisionBox.y + collisionBox.h >= 480){ // 480 = SCREEN_HEIGHT
        collisionBox.y = (collisionBox.y - (collisionBox.y - 480)) -
                          collisionBox.h;
    }    

    else if (collisionBox.y <= 0){
        collisionBox.y = 0;
    }
}

void Paddle::draw(SDL_Renderer *renderer)
{
    SDL_Rect renderQuad = {(int)collisionBox.x, (int)collisionBox.y,
                            collisionBox.w, collisionBox.h};
    SDL_RenderCopy(renderer, image, NULL, &renderQuad);
}

void Paddle::free()
{
    //free(image);
}