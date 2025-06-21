#pragma once
#include "stack.h"
#include <raylib.h>
#include <stdint.h>
#include <time.h>

#define REGISTER_COUNT 16
#define MEMORY_SIZE 4096

/* instructiosn per second */
#define INSTRUCTION_SPEED CLOCKS_PER_SEC / 700

static Sound beep;

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
    /* instruction clock */
    clock_t clock;
} chip8;

void load_font(chip8* vm);
chip8* load_rom(const char* path);
void print_memory(const chip8* vm);
void free_vm(chip8* vm);
void delay_timer(chip8* vm);

void fetch(chip8* vm);
