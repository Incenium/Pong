#include "ball.h"

void Ball::init(int x, int y, SDL_Texture *iimage)
{
    collisionBox.x = x;
    collisionBox.y = y;
    collisionBox.w = 32;
    collisionBox.h = 32;

    image = &*iimage;
}

void Ball::update(int delta, Paddle *paddle1, Paddle *paddle2)
{
    /* the player scores and the score string from main.cpp */
    extern int scorep1;
    extern int scorep2;

    extern std::ostringstream scoreString;

    collisionBox.x += xVel * (delta / 1000.0f);

    SDL_Rect intersection;

    if (SDL_IntersectRect(&collisionBox, &paddle1->collisionBox, &intersection) == SDL_TRUE){
        collisionBox.x = collisionBox.x + intersection.w;

        /*if (SDL_HasIntersection(&collisionBox, &paddle1->collisionBox) == SDL_TRUE){
            collisionBox.y = collisionBox.y + intersection.h;
        }*/

        xVel = xVel + (xVel * -2);
    }

    else if (SDL_IntersectRect(&collisionBox, &paddle2->collisionBox, &intersection) == SDL_TRUE){
        collisionBox.x = collisionBox.x - intersection.w;

        /*if (SDL_HasIntersection(&collisionBox, &paddle2->collisionBox) == SDL_TRUE){
            collisionBox.y = collisionBox.y - intersection.h;
        }*/

        xVel = xVel - (xVel * 2);
    }

    if (collisionBox.x + collisionBox.w >= 640){
        collisionBox.x = 200;
        collisionBox.y = 300;

        xVel = xVel - (xVel * 2);
        yVel = 200.0f;

        scorep1++;

        scoreString.str("");;
        scoreString << "Score p1: " << scorep1 << " | " << "score p2: " << scorep2;

        SDL_Delay(500);
    }

    else if (collisionBox.x <= 0){
        collisionBox.x = 200;
        collisionBox.y = 300;

        xVel = xVel + (xVel * -2);
        yVel = 200.0f;

        scorep2++;

        scoreString.str("");
        scoreString << "Score p1: " << scorep1 << " | " << "score p2: " << scorep2;

        SDL_Delay(500);
    }

    collisionBox.y += yVel * (delta / 1000.0f);

    if (SDL_IntersectRect(&collisionBox, &paddle1->collisionBox, &intersection) == SDL_TRUE){
            collisionBox.y = collisionBox.y - intersection.h;
        }

    else if (SDL_IntersectRect(&collisionBox, &paddle2->collisionBox, &intersection) == SDL_TRUE){
            collisionBox.y = collisionBox.y + intersection.h;
        }

    if (collisionBox.y + collisionBox.h >= 480){
        yVel = yVel - (yVel * 2);
    }

    else if (collisionBox.y <= 0){
        yVel = yVel + (yVel * -2);
    }

}

void Ball::draw(SDL_Renderer *renderer)
{
    SDL_Rect renderQuad = {collisionBox.x, collisionBox.y, 32, 32};

    SDL_RenderCopy(renderer, image, NULL, &renderQuad);
}

void Ball::free()
{

}