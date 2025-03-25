#include "display.h"
#include "stack.h"
#include "vm.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
    InitWindow(640, 320, "CHIP-8 Emulator");
    SetTargetFPS(60);

    chip8* vm = load_rom("15 Puzzle [Roger Ivie].ch8");
    int font = 0;
    while (!WindowShouldClose()) {

        BeginDrawing();
        clear_display();
        test_font(vm, font);
        font++;
        font %= 16;
        draw_display();
        EndDrawing();
    }

    free(vm);
    CloseWindow();

    return 0;
}
