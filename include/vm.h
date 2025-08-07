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
void sound_timer(chip8* vm);
void play_sound(chip8* vm);

void fetch(chip8* vm);

/**
 * Maps CHIP-8 keypad (0x0-0xF) to raylib keys
 *  1 2 3 C -> 1 2 3 4
 *  4 5 6 D -> Q W E R
 *  7 8 9 E -> A S D F
 *  A 0 B F -> Z X C V
 **/

static const int key_map[16] = {
    KEY_X, // 0
    KEY_ONE, // 1
    KEY_TWO, // 2
    KEY_THREE, // 3
    KEY_Q, // 4
    KEY_W, // 5
    KEY_E, // 6
    KEY_A, // 7
    KEY_S, // 8
    KEY_D, // 9
    KEY_Z, // A
    KEY_C, // B
    KEY_FOUR, // C
    KEY_R, // D
    KEY_F, // E
    KEY_V // F
};
