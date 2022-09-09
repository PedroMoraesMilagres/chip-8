#include <cstdio>
#include <cstdlib>
#include "include/display.hpp"
#include "src/chip8.cpp"

int main(int argc, char* argv[])
{
    Display display;

    Chip8 chip8;

    chip8.LoadROM(argv[1]);

    while (true)
    { 
        chip8.EmulateCycle();

        display.KeysInput();
    };

    return 0;
};
