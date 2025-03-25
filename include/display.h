#pragma once
#include "vm.h"
#include <stdint.h>

static uint8_t display[64][32] = { 0 };

void draw_display(void);
void clear_display(void);
void set_pixel(int x, int y);
void test_font(chip8* vm, int font);
