#include "vm.h"
#include "display.h"
#include "stack.h"
#include <fenv.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static clock_t delay_timer_prev;
static clock_t sound_timer_prev;

void free_vm(chip8* vm)
{
    free_stack(vm->stack);
    free(vm);
}

/* load rom into memory from 1FF onwards */
void load_font(chip8* vm)
{
    static const uint8_t chip8_font[] = {
        /* 0 */
        0xF0, 0x90, 0x90, 0x90, 0xF0,
        /* 1 */
        0x20, 0x60, 0x20, 0x20, 0x70,
        /* 2 */
        0xF0, 0x10, 0xF0, 0x80, 0xF0,
        /* 3 */
        0xF0, 0x10, 0xF0, 0x10, 0xF0,
        /* 4 */
        0x90, 0x90, 0xF0, 0x10, 0x10,
        /* 5 */
        0xF0, 0x80, 0xF0, 0x10, 0xF0,
        /* 6 */
        0xF0, 0x80, 0xF0, 0x90, 0xF0,
        /* 7 */
        0xF0, 0x10, 0x20, 0x40, 0x40,
        /* 8 */
        0xF0, 0x90, 0xF0, 0x90, 0xF0,
        /* 9 */
        0xF0, 0x90, 0xF0, 0x10, 0xF0,
        /* a */
        0xF0, 0x90, 0xF0, 0x90, 0x90,
        /* b */
        0xE0, 0x90, 0xE0, 0x90, 0xE0,
        /* c */
        0xF0, 0x80, 0x80, 0x80, 0xF0,
        /* d */
        0xE0, 0x90, 0x90, 0x90, 0xE0,
        /* e */
        0xF0, 0x80, 0xF0, 0x80, 0xF0,
        /* f */
        0xF0, 0x80, 0xF0, 0x80, 0x80
    };
    for (size_t i = 0; i < sizeof(chip8_font); i++) {
        vm->memory[i] = chip8_font[i];
    }
}

/* load chip8 file into vm and return vm */
chip8* load_rom(const char* path)
{
    chip8* vm = malloc(sizeof(chip8));
    vm->sp = 0;
    vm->ir = 0;
    vm->dt = 0;
    vm->st = 0;
    vm->stack = init_stack();
    vm->clock = clock();
    delay_timer_prev = clock();
    sound_timer_prev = clock();
    beep = LoadSound("resources/beep-329314.mp3");

    for (int i = 0; i < MEMORY_SIZE; i++) {
        vm->memory[i] = 0;
    }
    load_font(vm);

    /* old chip8 programs start at 0x200 */
    vm->pc = 0x200;

    FILE* file = fopen(path, "rb");
    if (!file) {
        perror("Error: cannot open file");
        exit(EXIT_FAILURE);
    }
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (vm->pc >= MEMORY_SIZE) {
            perror("Error: rom too large");
            exit(EXIT_FAILURE);
        }
        vm->memory[vm->pc++] = c;
    }
    vm->pc = 0x200;
    return vm;
}

void print_memory(const chip8* vm)
{
    printf("Memory dump:\n");
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (i % 16 == 0) {
            printf("\n");
        }
        printf("%02x ", vm->memory[i]);
    }
    printf("\n");
}

void delay_timer(chip8* vm)
{
    clock_t current = clock();
    int decrement = (current - delay_timer_prev) >= CLOCKS_PER_SEC ? 1 : 0;
    vm->dt -= vm->dt > 0 ? decrement : 0;
    delay_timer_prev = current;
}

void sound_timer(chip8* vm)
{
    clock_t current = clock();
    int decrement = (current - sound_timer_prev) >= CLOCKS_PER_SEC ? 1 : 0;
    vm->st -= vm->st > 0 ? decrement : 0;
    sound_timer_prev = current;
}

void play_sound(chip8* vm)
{
    if (vm->st > 0)
        PlaySound(beep);
}

void execute(chip8* vm, uint16_t instruction)
{
    printf("Fetched opcode: 0x%X\n", instruction);
    uint8_t x = (instruction & 0x0F00) >> 8;
    uint8_t y = (instruction & 0x00F0) >> 4;
    uint8_t n = instruction & 0x000F;
    uint8_t nn = instruction & 0x00FF;
    uint16_t nnn = instruction & 0x0FFF;
    printf("Executing opcode: 0x%X\n", instruction);
    switch (instruction & 0xF000) {
    case 0x0000: {
        /* ----- sys calls ----- */
        switch (nn & 0xE0) {
            clear_display();
            break;
        }
        break;
    }
    case 1:
        vm->pc = nn;
        break;
    case 6:
        vm->registers[x] = nn;
        break;
    case 'A':
        vm->ir = nnn;
        break;
    case 'D':
    }
}

void fetch(chip8* vm)
{

    clock_t current = clock();
    if ((current - vm->clock) >= INSTRUCTION_SPEED) {
        uint16_t instruction = (vm->memory[vm->pc] << 8) | vm->memory[vm->pc + 1];
        execute(vm, instruction);
        vm->pc += 2;
        vm->clock += INSTRUCTION_SPEED;
    }
}
