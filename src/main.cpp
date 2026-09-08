#include "constant.hpp"
#include "game_data.hpp"

int main() {
    InitWindow(WIDTH, HEIGHT, TITLE.data());
    InitAudioDevice();
    SetTargetFPS(FPS);
    GameData game_data;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        game_data.update();
        game_data.render();
        EndDrawing();
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
