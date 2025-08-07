#include "vm.h"
#include "display.h"
#include "raylib.h"
#include "stack.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TIMER_INTERVAL (CLOCKS_PER_SEC / 60)

void free_vm(chip8* vm)
{
    free_stack(vm->stack);
    free(vm);
}

const char* disassemble(uint16_t instruction)
{
    static char buf[32];

    uint8_t x = (instruction & 0x0F00) >> 8;
    uint8_t y = (instruction & 0x00F0) >> 4;
    uint8_t n = instruction & 0x000F;
    uint8_t nn = instruction & 0x00FF;
    uint16_t nnn = instruction & 0x0FFF;

    switch (instruction & 0xF000) {
    case 0x0000:
        switch (nn) {
        case 0xE0:
            return "CLS";
        case 0xEE:
            return "RET";
        default:
            return "SYS addr"; // 0NNN: System call (usually ignored)
        }
    case 0x1000:
        sprintf(buf, "JP 0x%03X", nnn);
        return buf;
    case 0x2000:
        sprintf(buf, "CALL 0x%03X", nnn);
        return buf;
    case 0x3000:
        sprintf(buf, "SE V%X, 0x%02X", x, nn);
        return buf;
    case 0x4000:
        sprintf(buf, "SNE V%X, 0x%02X", x, nn);
        return buf;
    case 0x5000:
        sprintf(buf, "SE V%X, V%X", x, y);
        return buf;
    case 0x6000:
        sprintf(buf, "LD V%X, 0x%02X", x, nn);
        return buf;
    case 0x7000:
        sprintf(buf, "ADD V%X, 0x%02X", x, nn);
        return buf;
    case 0x8000:
        switch (n) {
        case 0:
            sprintf(buf, "LD V%X, V%X", x, y);
            return buf;
        case 1:
            sprintf(buf, "OR V%X, V%X", x, y);
            return buf;
        case 2:
            sprintf(buf, "AND V%X, V%X", x, y);
            return buf;
        case 3:
            sprintf(buf, "XOR V%X, V%X", x, y);
            return buf;
        case 4:
            sprintf(buf, "ADD V%X, V%X", x, y);
            return buf; // 8XY4: Set VX = VX + VY, VF = carry
        case 5:
            sprintf(buf, "SUB V%X, V%X", x, y);
            return buf;
        case 6:
            sprintf(buf, "SHR V%X", x);
            return buf;
        case 7:
            sprintf(buf, "SUBN V%X, V%X", x, y);
            return buf;
        case 0xE:
            sprintf(buf, "SHL V%X", x);
            return buf;
        default:
            sprintf(buf, "Unknown 8XYN: 0x%X", instruction);
            return buf;
        }
    case 0x9000:
        sprintf(buf, "SNE V%X, V%X", x, y);
        return buf;
    case 0xA000:
        sprintf(buf, "LD I, 0x%03X", nnn);
        return buf;
    case 0xB000:
        sprintf(buf, "JP V0, 0x%03X", nnn);
        return buf;
    case 0xC000:
        sprintf(buf, "RND V%X, 0x%02X", x, nn);
        return buf;
    case 0xD000:
        sprintf(buf, "DRW V%X, V%X, %d", x, y, n);
        return buf;
    case 0xE000:
        switch (nn) {
        case 0x9E:
            sprintf(buf, "SKP V%X", x);
            return buf;
        case 0xA1:
            sprintf(buf, "SKNP V%X", x);
            return buf;
        default:
            sprintf(buf, "Unknown EXNN: 0x%X", instruction);
            return buf;
        }
    case 0xF000:
        switch (nn) {
        case 0x07:
            sprintf(buf, "LD V%X, DT", x);
            return buf;
        case 0x0A:
            sprintf(buf, "LD V%X, K", x);
            return buf;
        case 0x15:
            sprintf(buf, "LD DT, V%X", x);
            return buf;
        case 0x18:
            sprintf(buf, "LD ST, V%X", x);
            return buf;
        case 0x1E:
            sprintf(buf, "ADD I, V%X", x);
            return buf;
        case 0x29:
            sprintf(buf, "LD F, V%X", x);
            return buf;
        case 0x33:
            sprintf(buf, "LD B, V%X", x);
            return buf;
        case 0x55:
            sprintf(buf, "LD [I], V%X", x);
            return buf;
        case 0x65:
            sprintf(buf, "LD V%X, [I]", x);
            return buf;
        default:
            sprintf(buf, "Unknown FXNN: 0x%X", instruction);
            return buf;
        }
    default:
        sprintf(buf, "Unknown opcode: 0x%X", instruction);
        return buf;
    }
}
void load_font(chip8* vm)
{
    static const uint8_t chip8_font[] = {
        /* 0 */ 0xF0, 0x90, 0x90, 0x90, 0xF0,
        /* 1 */ 0x20, 0x60, 0x20, 0x20, 0x70,
        /* 2 */ 0xF0, 0x10, 0xF0, 0x80, 0xF0,
        /* 3 */ 0xF0, 0x10, 0xF0, 0x10, 0xF0,
        /* 4 */ 0x90, 0x90, 0xF0, 0x10, 0x10,
        /* 5 */ 0xF0, 0x80, 0xF0, 0x10, 0xF0,
        /* 6 */ 0xF0, 0x80, 0xF0, 0x90, 0xF0,
        /* 7 */ 0xF0, 0x10, 0x20, 0x40, 0x40,
        /* 8 */ 0xF0, 0x90, 0xF0, 0x90, 0xF0,
        /* 9 */ 0xF0, 0x90, 0xF0, 0x10, 0xF0,
        /* a */ 0xF0, 0x90, 0xF0, 0x90, 0x90,
        /* b */ 0xE0, 0x90, 0xE0, 0x90, 0xE0,
        /* c */ 0xF0, 0x80, 0x80, 0x80, 0xF0,
        /* d */ 0xE0, 0x90, 0x90, 0x90, 0xE0,
        /* e */ 0xF0, 0x80, 0xF0, 0x80, 0xF0,
        /* f */ 0xF0, 0x80, 0xF0, 0x80, 0x80
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
    InitAudioDevice();
    beep = LoadSound("../resources/beep-329314.wav");

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
    if (vm->dt > 0) {
        vm->dt--;
    }
}

void sound_timer(chip8* vm)
{
    if (vm->st > 0) {
        vm->st--;
    }
}

void play_sound(chip8* vm)
{
    if (vm->st > 0)
        PlaySound(beep);
}

void draw(chip8* vm, uint8_t x, uint8_t y, uint8_t n)
{
    uint16_t sprite_address = vm->ir;
    uint8_t start_x = vm->registers[x] % 64;
    uint8_t start_y = vm->registers[y] % 32;
    uint8_t sprite_height = n;

    vm->registers[0xF] = 0;

    for (int y_line = 0; y_line < sprite_height; y_line++) {
        uint8_t current_y = start_y + y_line;
        if (current_y >= 32)
            break;

        uint8_t sprite_byte = vm->memory[sprite_address + y_line];

        for (int x_line = 0; x_line < 8; x_line++) {
            uint8_t current_x = start_x + x_line;
            if (current_x >= 64)
                break;

            if (sprite_byte & (0x80 >> x_line)) {
                if (set_pixel(current_x, current_y)) {
                    vm->registers[0xF] = 1;
                }
            }
        }
    }
}

void lm_operations(chip8* vm, uint8_t x, uint8_t y, uint8_t n)
{
    switch (n) {
    case 0x0000:
        vm->registers[x] = vm->registers[y];
        break;
    case 0x0001:
        vm->registers[x] |= vm->registers[y];
        break;
    case 0x0002:
        vm->registers[x] &= vm->registers[y];
        break;
    case 0x0003:
        vm->registers[x] ^= vm->registers[y];
        break;
    case 0x0004: {
        uint16_t addition = vm->registers[x] + vm->registers[y];
        vm->registers[0xF] = (addition > 255) ? 1 : 0;
        vm->registers[x] = (uint8_t)addition;
        break;
    }
    case 0x0005: {
        vm->registers[0xF] = (vm->registers[x] > vm->registers[y]) ? 1 : 0;
        vm->registers[x] -= vm->registers[y];
        break;
    }
    case 0x0006: // SHR (Shift Right) - Using legacy behavior
        vm->registers[0xF] = vm->registers[y] & 0x01;
        vm->registers[x] = vm->registers[y] >> 1;
        break;
    case 0x0007: {
        vm->registers[0xF] = (vm->registers[y] > vm->registers[x]) ? 1 : 0;
        vm->registers[x] = vm->registers[y] - vm->registers[x];
        break;
    }
    case 0x000E: // SHL (Shift Left) - Using legacy behavior
        vm->registers[0xF] = (vm->registers[y] & 0x80) >> 7;
        vm->registers[x] = vm->registers[y] << 1;
        break;
    }
}

void keyboard_check(chip8* vm, uint8_t x, uint16_t nn)
{
    uint8_t key_value = vm->registers[x];
    if (key_value > 0xF)
        return;

    int raylib_key = key_map[key_value];
    switch (nn) {
    case 0x009E:
        if (IsKeyDown(raylib_key)) {
            vm->pc += 2;
        }
        break;
    case 0x00A1:
        if (!IsKeyDown(raylib_key)) {
            vm->pc += 2;
        }
        break;
    }
}

bool execute_fx_opcodes(chip8* vm, uint8_t x, uint16_t nn)
{
    switch (nn) {
    case 0x07:
        vm->registers[x] = vm->dt;
        break;
    case 0x0A: {
        int raylib_key = GetKeyPressed();

        if (raylib_key == 0) {
            vm->pc -= 2;
        } else {
            bool key_found = false;
            for (int i = 0; i < 16; i++) {
                if (key_map[i] == raylib_key) {
                    vm->registers[x] = i;
                    key_found = true;
                    break;
                }
            }
            if (!key_found) {
                vm->pc -= 2;
            }
        }
        break;
    }
    case 0x15:
        vm->dt = vm->registers[x];
        break;
    case 0x18:
        vm->st = vm->registers[x];
        break;
    case 0x1E:
        vm->ir += vm->registers[x];
        break;
    case 0x29:
        vm->ir = vm->registers[x] * 5;
        break;
    case 0x33: {
        uint8_t value = vm->registers[x];
        vm->memory[vm->ir + 2] = value % 10;
        value /= 10;
        vm->memory[vm->ir + 1] = value % 10;
        value /= 10;
        vm->memory[vm->ir] = value % 10;
        break;
    }
    case 0x55:
        for (int i = 0; i <= x; i++) {
            vm->memory[vm->ir + i] = vm->registers[i];
        }
        break;
    case 0x65:
        for (int i = 0; i <= x; i++) {
            vm->registers[i] = vm->memory[vm->ir + i];
        }
        break;
    }
    return true;
}

// Main execution function
bool execute(chip8* vm, uint16_t instruction)
{
    uint8_t x = (instruction & 0x0F00) >> 8;
    uint8_t y = (instruction & 0x00F0) >> 4;
    uint8_t n = instruction & 0x000F;
    uint8_t nn = instruction & 0x00FF;
    uint16_t nnn = instruction & 0x0FFF;

    switch (instruction & 0xF000) {
    case 0x0000:
        switch (nn) {
        case 0x00E0:
            clear_display();
            break;
        case 0x00EE:
            vm->pc = pop(vm->stack);
            return false;
        }
        break;
    case 0x1000:
        vm->pc = nnn;
        return false;
    case 0x2000:
        push(vm->stack, vm->pc + 2);
        vm->pc = nnn;
        return false;
    case 0x3000:
        if (vm->registers[x] == nn)
            vm->pc += 2;
        break;
    case 0x4000:
        if (vm->registers[x] != nn)
            vm->pc += 2;
        break;
    case 0x5000:
        if (vm->registers[x] == vm->registers[y])
            vm->pc += 2;
        break;
    case 0x6000:
        vm->registers[x] = nn;
        break;
    case 0x7000:
        vm->registers[x] += nn;
        break;
    case 0x8000:
        lm_operations(vm, x, y, n);
        break;
    case 0x9000:
        if (vm->registers[x] != vm->registers[y])
            vm->pc += 2;
        break;
    case 0xA000:
        vm->ir = nnn;
        break;
    case 0xB000:
        vm->pc = nnn + vm->registers[0];
        return false;
    case 0xC000:
        vm->registers[x] = rand() & nn;
        break;
    case 0xD000:
        draw(vm, x, y, n);
        break;
    case 0xE000:
        keyboard_check(vm, x, nn);
        break;
    case 0xF000:
        execute_fx_opcodes(vm, x, nn);
        break;
    default:
        printf("Error: Unknown opcode 0x%X\n", instruction);
        break;
    }
    return true;
}

void fetch(chip8* vm)
{
    uint16_t instruction = (vm->memory[vm->pc] << 8) | vm->memory[vm->pc + 1];
    if (execute(vm, instruction)) {
        vm->pc += 2;
    }
}
