#include "display.h"
#include <raylib.h>

#define FONT_START 5
#define BYTE_WIDTH 0x80

// Corrected draw_display
void draw_display(void)
{
    ClearBackground(BLACK);

    for (int y = 0; y < 32; y++) { // Y loop should be outer for rows
        for (int x = 0; x < 64; x++) { // X loop should be inner for columns
            if (display[y][x] == 1) { // CORRECT: display[y][x]
                DrawRectangle(x * 10, y * 10, 10, 10, WHITE);
            }
        }
    }
}

// Corrected clear_display
void clear_display(void)
{
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            display[y][x] = 0; // CORRECT: display[y][x]
        }
    }
}

bool set_pixel(int x, int y)
{
    // Prevent drawing outside the screen bounds
    if (x < 0 || x >= 64 || y < 0 || y >= 32) {
        return false;
    }

    // Array access is still [row][col], which is [y][x]
    bool collision = (display[y][x] == 1);

    display[y][x] ^= 1; // Flip the pixel

    return collision; // Return true if a pixel was erased
}

void test_font(chip8* vm, int font)
{
    uint8_t* character = &vm->memory[font * FONT_START];
    for (int row = 0; row < 5; row++) {
        uint8_t byte = character[row];

        for (int col = 0; col < 8; col++) {
            if ((byte & (BYTE_WIDTH >> col)) != 0) {
                display[col][row] = 1;
            }
        }
    }
}
