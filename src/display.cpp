#pragma once

#include <cstdlib>
#include "../include/display.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_shape.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>


int WIDTH = 800;
int HEIGHT = 600;


Display::Display()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
            WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
};

Display::~Display()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
};

