#pragma once
#include "stack.h"
#include <stdint.h>

#define REGISTER_COUNT 16
#define MEMORY_SIZE 4096

typedef struct chip8 {
    /* program counter */
    uint16_t pc;
    /* stack pointer */
    uint8_t sp;
    /* index register */
    uint16_t ir;
    /* V0 - VF */
    uint8_t registers[REGISTER_COUNT];
    /* 4kb of ram */
    uint8_t memory[MEMORY_SIZE];
    stack* stack;
    /* delay timer */
    uint8_t dt;
    /* sound timer */
    uint8_t st;
} chip8;

void load_font(chip8* vm);
chip8* load_rom(const char* path);
void print_memory(const chip8* vm);
