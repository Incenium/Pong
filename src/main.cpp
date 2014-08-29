#include "timer.h"
#include "paddle.h"
#include "ball.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int FPS = 60;

int scorep1 = 0;
int scorep2 = 0;

std::ostringstream scoreString;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Texture* Paddletexture = NULL;
SDL_Texture* Balltexture = NULL;

TTF_Font *font = NULL;
SDL_Texture* Scoretexture = NULL;

SDL_Event event;

bool quit = false;

Timer timer;
Timer delta;

Paddle paddle1;
Paddle paddle2;
Ball ball;

bool init()
{
    bool success = true;

    /* Initialize SDL subsystems below */

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize video! SDL Error: %s\n", 
               SDL_GetError());

        success = false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)){
        printf("SDL_Image could not initalize! SDL_Image Error: %s\n",
               IMG_GetError());

        success = false;
    }

    if (TTF_Init() == -1){
        printf("SDL_ttf could not initalize! SDL_ttf Error: %s\n",
               TTF_GetError());

        success = false;
    }

    window = SDL_CreateWindow("Block Blast", 
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, 
                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL){
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());

        success = false;
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "100"))
    {
        printf( "Warning: Linear texture filtering not enabled!" );
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL){
        printf("Renderer could not be created! SDL Error: %s", SDL_GetError());
        success = false;
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    /* Initialize game objects such as players, maps, enemies, etc below */

    //paddle.init(100, texture);

    return success;
}

SDL_Texture* load_texture(std::string path)
{
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    SDL_Texture* newTexture = NULL;

    if (loadedSurface == NULL){
        printf("Unable to load image %s! IMG_Error: %s\n", path.c_str(),
               SDL_GetError());
    }

    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    
    if (newTexture == NULL){
        printf("Unable to create texture from image %s! SDL_Error: %s\n",
               path.c_str(), SDL_GetError());
    }

    SDL_FreeSurface(loadedSurface);

    return newTexture;
}

SDL_Texture* load_text(std::string text, SDL_Color textColor)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Texture* texture;

    if (textSurface == NULL){
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }

    else {
        texture = SDL_CreateTextureFromSurface(renderer, textSurface);

        if (texture == NULL){
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }

        SDL_FreeSurface(textSurface);
    }

    return texture;
}

bool load_resources()
{
    bool success = true;

    font = TTF_OpenFont("resources/pixelfont.ttf", 28);

    if (font == NULL){
        printf("Failed to load pixel font! SDL_ttf Error: %s\n",
               TTF_GetError());

        success = false;
    }

    SDL_Color textColor = {0, 0, 0};

    Scoretexture = load_text("this is text yo", textColor);

    Paddletexture = load_texture("resources/iblock2.png");
    Balltexture = load_texture("resources/bird.png");

    if (Paddletexture == NULL){
        printf("Could not load texture! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    if (Balltexture == NULL){
        printf("Could not load texture! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    paddle1.init(100, Paddletexture);
    paddle2.init(540, Paddletexture);
    ball.init(200, 300, Balltexture);

    return success;
}

/* update polls events, updates player position, enemy positions, etc */
SDL_Color textColor = {0, 0, 0};
void update(Timer* delta)
{
    while (SDL_PollEvent(&event) != 0){
        if (event.type == SDL_QUIT){
            quit = true;
        }

        paddle1.handleInput(event);
        paddle2.handleInput(event);
    }

    paddle1.update(delta->get_ticks());
    paddle2.update(delta->get_ticks());
    ball.update(delta->get_ticks(), &paddle1, &paddle2);
    Scoretexture = load_text(scoreString.str().c_str(), textColor);
    delta->start();
}

/* render draws the updated map, characters, enemies, etc to the screen */
void render()
{
    SDL_RenderClear(renderer);

    SDL_Rect pos;

    pos.x = 165;
    pos.y = 50;
    pos.w = 300;
    pos.h = 30;

    //SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);

    paddle1.draw(renderer);
    paddle2.draw(renderer);
    ball.draw(renderer);

    SDL_RenderCopy(renderer, Scoretexture, NULL, &pos);

    SDL_RenderPresent(renderer);
}

void close()
{
    SDL_DestroyTexture(Paddletexture);
    Paddletexture = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    window = NULL;
    renderer = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    int frame = 0;

    scoreString << "Score p1: " << scorep1 << " | " << "score p2: " << scorep2;

    std::cout << scoreString.str() << '\n';

    if (!init()){
        printf("Failed to initialize!\n");

        return 1;
    }

    if (!load_resources()){
        printf("Failed to load resources!");

        return 1;
    }

    delta.start();
    while (!quit){
        timer.start();
        
        update(&delta);
        render();


        /* uncomment the following block to debug the framerate */
        
        /*frame++;

        if (frame == 60){
            printf("60 frames achieved\n");
            frame = 0;
        }*/

        if (timer.get_ticks() < 1000 / FPS){
            SDL_Delay((1000 / FPS) - timer.get_ticks());
        }
    }

    close();

    return 0;
}