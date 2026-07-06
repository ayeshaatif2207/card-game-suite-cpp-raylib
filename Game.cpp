#include "raylib.h"
#include "Game.h"



Game::~Game() {
}

void Game::run(int width, int height, const char* title) {
    InitWindow(width, height, title);
    SetTargetFPS(60);

    start();
    while (!WindowShouldClose()) {
        update();

        BeginDrawing();
        ClearBackground(DARKGREEN);
        draw();
        EndDrawing();
    }
    CloseWindow();
}
