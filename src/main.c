#include "raylib.h"

int main(void) {
    InitWindow(800, 600, "Artemis - Reingreso");  // abre la ventana
    SetTargetFPS(60);                             // 60 cuadros por segundo

    while (!WindowShouldClose()) {   // hasta que cierren la ventana o aprieten ESC
        // --- ACTUALIZAR ---

        // --- DIBUJAR ---
        BeginDrawing();
        ClearBackground(BLACK);      // borra el cuadro anterior
        DrawText("Bienvenidos a la Mision Artemis! :D", 190, 200, 20, LIGHTGRAY);
        // ... dibujos ...
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
