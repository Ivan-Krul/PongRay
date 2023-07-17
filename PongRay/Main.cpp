#include <iostream>
#include "raylib.h"

int main() {
    int windowWidth = 640;
    int windowHeight = 480;
    int textFont = 20;

    InitWindow(windowWidth, windowHeight, "Title");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", windowWidth / 5, windowHeight / 2 - textFont / 2, textFont, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
