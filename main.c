#include "raylib.h"
#include "game.h"
#include "menu.h"

typedef enum {
    MENU,
    PLAYING
} GameState;

int main(void)
{
    InitWindow(800, 600, "Plinko Arcade");
    SetTargetFPS(60);

    GameState state = MENU;
    initGame();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (state == MENU) {
            if (updateMenu())
                state = PLAYING;
            drawMenu();
        }
        else if (state == PLAYING) {
            updateGame();
            drawGame();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
