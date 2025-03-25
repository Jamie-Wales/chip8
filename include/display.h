#pragma once
#include "vm.h"
#include <stdint.h>

#define WIDTH 64
#define HEIGHT 32

static uint8_t display[HEIGHT][WIDTH] = { 0 };

void draw_display(void);
void clear_display(void);
void set_pixel(int x, int y);
void test_font(chip8* vm, int font);
