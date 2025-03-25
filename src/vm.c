#include "vm.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

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
