#pragma once
#include <stdint.h>

typedef struct chip8 {
    /* program counter */
    uint16_t pc;
    /* stack pointer */
    uint8_t sp;
    /* index register */
    uint16_t ir;
    /* V0 - VF */
    uint8_t registers[16];
    /* 4kb of ram */
    uint8_t memory[4096];
    uint16_t stack[16];
    /* delay timer */
    uint8_t dt;
    /* sound timer */
    uint8_t st;
} chip8;

void load_font(chip8* vm);
chip8* load_rom(const char* path);
void print_memory(const chip8* vm);
