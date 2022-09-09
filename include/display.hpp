#ifndef DISPLAY
#define DISPLAY

#pragma once


#include <SDL2/SDL.h>


class Display
{
    public:
        ~Display();
        Display();
        void KeysInput();
    private:
    	  SDL_Window* window{};
	      SDL_Renderer* renderer{};
	      SDL_Texture* texture{};
};
#endif
