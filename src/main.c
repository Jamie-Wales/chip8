#include "display.h"
#include "vm.h"
#include <raylib.h>

int main(void)
{
    InitWindow(640, 320, "CHIP-8 Emulator");
    SetTargetFPS(60);

    chip8* vm = load_rom("IBM Logo.ch8");
    while (!WindowShouldClose()) {
        fetch(vm);
        BeginDrawing();
        draw_display();
        EndDrawing();
    }

    free_vm(vm);
    CloseWindow();

    return 0;
}
