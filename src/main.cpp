#include "constant.hpp"
#include "game_data.hpp"

int main() {
    InitWindow(WIDTH, HEIGHT, TITLE.data());
    SetTargetFPS(FPS);
    GameData game_data;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        game_data.update();
        game_data.render();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
