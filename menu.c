//
// Created by ASMIRAY on 26-01-2026.
//
#include "menu.h"

static Rectangle startBtn = {300, 250, 200, 50};
static Rectangle quitBtn  = {300, 320, 200, 50};

bool updateMenu(void)
{
    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, startBtn) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return true;

    if (CheckCollisionPointRec(mouse, quitBtn) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        CloseWindow();

    return false;
}

void drawMenu(void)
{
    DrawText("PLINKO ARCADE", 260, 150, 40, BLACK);

    DrawRectangleRec(startBtn, LIGHTGRAY);
    DrawText("START", 360, 265, 20, BLACK);

    DrawRectangleRec(quitBtn, LIGHTGRAY);
    DrawText("QUIT", 370, 335, 20, BLACK);
}
