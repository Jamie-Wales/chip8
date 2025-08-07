#include "display.h"
#include "vm.h"
#include <raylib.h>
#include <stdio.h>
#define CYCLES_PER_FRAME 20

int main(void)
{
    InitWindow(640, 320, "CHIP-8 Emulator");
    SetTargetFPS(60);

    chip8* vm = load_rom("Invaders.ch8");
    if (vm == NULL) {
        return 1;
    }

    while (!WindowShouldClose()) {
        for (int i = 0; i < CYCLES_PER_FRAME; i++) {
            fetch(vm);
        }

        delay_timer(vm);
        sound_timer(vm);
        play_sound(vm);

        BeginDrawing();
        ClearBackground(BLACK);
        draw_display();
        EndDrawing();
    }

    free_vm(vm);
    CloseWindow();

    return 0;
}
