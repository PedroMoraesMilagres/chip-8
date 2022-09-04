#pragma once

#include <cstdlib>
#include <SDL2/SDL.h>



class Display
{
    public:
        Display();
        ~Display();
        void KeysInput();
        void Update();
    private:
    	  SDL_Window* window{};
	      SDL_Renderer* renderer{};
	      SDL_Texture* texture{};
};

