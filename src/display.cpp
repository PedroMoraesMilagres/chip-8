#pragma once

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <cstdlib>
#include "../include/display.hpp"
#include "../src/chip8.cpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_shape.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>
#include <endian.h>


int WIDTH = 800;
int HEIGHT = 600;

Chip8 chip8;

Display::Display()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
            WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
};

Display::~Display()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_Quit();
};

void Display::KeysInput()
{ 
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) exit(0);

        if (event.type == SDL_KEYDOWN)
        { 
            if (event.key.keysym.sym == SDLK_ESCAPE) exit(0);
            
            for (int i = 0; i < 16; i++)
            {
                if (event.key.keysym.sym == chip8.keypad[i])
                {
                    chip8.keypad[i] = 1;
                };
            };
        
        if (event.type == SDL_KEYUP)
            for (int i = 0; i < 16; i++)
            {
                if (event.key.keysym.sym == chip8.keypad[i])
                {
                    chip8.keypad[i] = 1;
                };
            };

        };
    };
};

void Display::Update(void const* buffer, int pitch)
{
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
};
