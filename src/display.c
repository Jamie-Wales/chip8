#include "display.h"
#include <raylib.h>

void draw_display(void)
{
    ClearBackground(BLACK);

    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 32; y++) {
            if (display[x][y] == 1) {
                DrawRectangle(x * 10, y * 10, 10, 10, WHITE);
            }
        }
    }
}

void clear_display(void)
{
    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 32; y++) {
            display[x][y] = 0;
        }
    }
}

void set_pixel(int x, int y)
{
    display[x][y] = display[x][y] ^ display[x][y];
}

void test_font(chip8* vm, int font)
{
    uint8_t* character = &vm->memory[font * 5];
    for (int row = 0; row < 5; row++) {
        uint8_t byte = character[row];

        for (int col = 0; col < 8; col++) {
            if ((byte & (0x80 >> col)) != 0) {
                display[col][row] = 1;
            }
        }
    }
}
