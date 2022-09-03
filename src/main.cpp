#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "chip8.cpp"
#include <unistd.h>

int main(int argc, char* argv[])
{

    Chip8 chip8;
    chip8.LoadROM(argv[1]);
    
    while (true)
    {
        chip8.EmulateCycle();
    };

    return 0;
};
