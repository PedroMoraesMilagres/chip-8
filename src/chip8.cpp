#include <fstream>
#include <string.h>
#include <iostream>
#include "../include/fontset.hpp"

using Byte = unsigned char;
using Word = unsigned short;

struct Chip8
{
    Word memory[4096]; // 4Kb memory
    Word PC{}, I{}; // 16 bit program counter, 16 bit index register,
    Word stack[16]; // 16 bit stack
    Word opcode{}; // current instruction
    Byte display[64 * 32]{}; // pixel data
    Byte keypad[16]; // keypad
    Byte SP{}, DelayTimer{}; // 8 bit stack pointer, 8 bit delay timer
    Byte V[16]; // 8 bit 16 general purpose variable register
    bool drawFlag = false;

    void LoadFontset()
    {
        for (unsigned int i = 0; i < 80; ++i)
        {
            memory[0x50 + i] = Fontset[i];
        };
    };

    void LoadROM(const char* filename)
    {   
        std::ifstream file(filename, std::ios::binary | std::ios::in);

        if (file.good())
        {
            file.seekg(0, std::ios::end);
            size_t file_size = file.tellg();
            file.seekg(0, std::ios::beg);

            char* buffer = new char[file_size];

            file.read(buffer, file_size);
            file.close();

            if (file_size < (4096 - 512))
            {
                for (int i = 0; i < file_size; ++i)
                    memory[0x200 + i] = buffer[i];
            } else { 
                std::cerr << "The file is too big!" << std::endl;
            };

            delete[] buffer;        
        };
    };

    void EmulateCycle()
    {     
        opcode = memory[PC] << 8 | memory[PC + 1]; // fetch the instruction

        Byte VX = (opcode & 0x0F00) >> 8;
        Byte VY = (opcode & 0x00F0) >> 4;
        Byte N = opcode & 0x000F;
        Byte NN = opcode & 0x00FF;
        Word NNN = opcode & 0x0FFF;
        
        PC += 2;
        
        switch ((opcode & 0xF000) >> 12)
        {
            case 0x0:
                switch (NN)
                {
                    case 0xE0: // 00E0 - CLS
                        printf("%04X\tCLS", opcode);
                        memset(display, 0, sizeof display);
                    break;
                    
                    case 0xEE: // 00EE - RET
                        printf("%04X\tRET", opcode);
                    break;

                    default:
                        printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
                    break;
                };
            break;

            case 0x1: // 1nnn - JP addr
                printf("%04X\tJP $%03X\n", opcode, NNN);
                PC = NNN;
            break;

            case 0x2: // 2nnn - CALL addr
                printf("%04X\tCALL $%03X\n", opcode, NNN);
            break;

            case 0x3: // 3xnn - SE Vx, byte
                printf("%04X\tSE $%01X $%02X\n", opcode, VX, NN);
            break;

            case 0x4: // 4xnn - SNE Vx, byte    
                printf("%04X\tSNE $%01X $%03X\n", opcode, VX, NN);
            break;

            case 0x5: //5xy0 - SE Vx, Vy
                printf("%04X\tSE $%01X $%01X\n", opcode, VX, VY);
            break;

            case 0x6: // 6xnn - LD Vx, byte
                printf("%04X\tLD V$%01X $%02X\n", opcode, VX, NN);
                V[VX] = NN;
            break;

            case 0x7: // 7xnn - ADD Vx, byte
                printf("%04X\tADD V$%01X $%02X\n", opcode, VX, NN);

                V[VX] += NN;
            break;

            case 0x8:    
                switch(N)
                { 
                    case 0x0: // 8xy0 - LD VX, VY    
                        printf("%04X\tLD V$%01X V$%01X\n", opcode, VX, VY);
                    break;

                    case 0x1: // 8xy1 - OR VX, VY
                        printf("%04X\tOR V$%01X V$%1X\n", opcode, VX, VY);
                    break;

                    case 0x2: // 8xy2 - AND VX, VY
                        printf("%04X\tAND V$%01X V$%01X\n", opcode, VX, VY);
                    break;
                     
                    case 0x3: // 8xy3 - XOR VX, VY
                        printf("%04X\tXOR V$%01X V$%01X\n", opcode, VX, VY);
                    break;

                    case 0x4: // 8xy4 - ADD VX, VY
                        printf("%04X\tADD V$%01X V$%01X\n", opcode, VX, VY);
                    break;

                    case 0x5: // 8xy5 - SUB VX, VY    
                        printf("%04X\tSUB V$%01X, {V$%01X}\n", opcode, VX, VY);
                    break;

                    case 0x6: // 8xy6 - SHR VX, {, VY}
                        printf("%04X\tSHR V$%01X, {V$%01X}\n", opcode, VX, VY);
                    break;

                    case 0x7: // 8xy7 - SUBN VX, VY   
                        printf("%04X\tSUBN V$%01X V$%01X\n", opcode, VX, VY);
                    break;

                    case 0xE: // 8xyE - SHL VX, {, VY}  
                        printf("%04X\tSHL V$%01X, {V$%01X}\n", opcode, VX, VY);
                    break;

                    default:
                        printf("Unknown opcode [0x8000]: 0x%X\n", opcode);
                    break;
                };
            break;
            
            case 0x9: // 9xy0 - SNE VX, VY
                printf("%04X\tSNE V$%01X V$%01X\n", opcode, VX, VY);
            break;

            case 0xA: // Annn - LD I, byte 
                printf("%04X\tLD I$%03X $%03X\n", opcode, I, NNN);
            break;

            case 0xB: // Bnnn V0, addr
                printf("%04X\tJP V0 $%03X\n", opcode, NNN);
            break;

            case 0xC: // Cxnn - RND VX, byte
                printf("%04X\tRND V$%01X V$%01X\n", opcode, VX, VY);
            break;

            case 0xD: // Dxy0 - Draw, Vx, Vy, N
                printf("%04x\tDRW V$%01X V$%01X $%01X\n", opcode, VX, VY, N);
                
                V[0xF] = 0;
                Byte pixel;

                for (int row =0; row < N; ++row)
                {
                    pixel = memory[I + row];

                    for (int col = 0; col < 8; ++col)
                    {
                        if((pixel & (0x80 >> col)) != 0)
                        {
                            Byte X = V[VX] = VX % 64;
                            Byte Y = V[VY] = VY % 32;
                        
                        if (display[X + (Y * 64)])
                        {
                            V[0xF] = 1;
                        };
                        display[X * (Y * 64)] ^= 1;
                        };
                    };  
                };
                drawFlag = true;
            break;

            case 0xE:
                switch(NN)
                {
                    case 0x9E: // Ex9E - SKP VX
                        printf("%04X\tSKP V$%01X\n", opcode, VX);
                    break;            
                        
                    case 0xA1: // ExA1 - SKNP VX
                        printf("%04X\tSKNP V$%01X\n", opcode, VX);
                    break;

                    default:
                        printf("Unknown opcode [0xE000]: 0x%X\n", opcode);
                    break;
                };
            break;
            
            case 0xF:
                switch (NN)
                {
                    case 0x07: // Fx07 - LD VX, DT 
                        printf("%04X\tLD V$%01X, DT\n", opcode, VX);
                    break;

                    case 0x0A: // Fx0A - LD VX, K
                        printf("%04X\tSNE V$%01X\n", opcode, N);
                    break;

                    case 0x15: // Fx15 - LD DT, VX
                        printf("%04X\tLD DT, V$%01X\n", opcode, VX);
                    break;

                    case 0x18: // Fx18 - LD ST, VX
                        printf("%04X\tLD ST V$%01X\n", opcode, VX);
                    break;

                    case 0x1E: // Fx1E - ADD I, VX
                        printf("%04X\tADD I V$%01X\n", opcode, VX);
                    break;
   
                    case 0x29: // Fx29 - LD F, VX
                        printf("%04X\tLD F V$%01X\n", opcode, VX);
                    break;
    
                    case 0x33: // Fx33 - LD B, VX
                        printf("%04X\tLD B V$%01X\n", opcode, VX);
                    break;
                    
                    case 0x55: // Fx55 - LD [I], VX
                        printf("%04X\tLD V$%01X \n", opcode, VX);
                    break;

                    case 0x65: // Fx65 - LD VX, [I]
                        printf("%04X\tLD V$%01X [I]\n", opcode, VX);
                    break;

                    default:
                        printf("Unknown opcode [0xF000]: 0x%X\n", opcode);
                    break;
                };
            break;
        };

    };
    void UpdateTimers()
    {
    if (DelayTimer > 0)
        --DelayTimer;
    };
};

