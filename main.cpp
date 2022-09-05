#include <cstdio>
#include <cstdlib>
#include "include/display.hpp"
#include "src/display.cpp"


int main(int argc, char* argv[])
{
    
    Display display;

    chip8.LoadROM(argv[1]);
    
    int videoPitch = sizeof(chip8.display[0] * 600);

    while (true)
    {
        chip8.EmulateCycle();
        
        display.KeysInput();

        if (chip8.drawFlag)
        {
            chip8.drawFlag = false;
            
            display.Update(chip8.display, videoPitch);
        };
    };

    return 0;
};
