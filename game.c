//
// Created by ASMIRAY on 26-01-2026.
//

#include "game.h"
#include "raymath.h"
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_BALLS 100
#define PEG_COUNT 70
#define MINI_PEG_COUNT 10
#define BUCKET_COUNT 5

typedef struct {
    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color;
    bool active;
} Ball;

typedef struct {
    Vector2 position;
    float radius;
} Peg;

typedef struct {
    Rectangle rect;
    int score;
} Bucket;

// GAME STATE
static Ball balls[MAX_BALLS];
static int ballCount = 0;
static int ballsLeft = 10;
static int totalScore = 0;

static Peg pegs[PEG_COUNT];
static Peg miniPegs[MINI_PEG_COUNT];
static Bucket buckets[BUCKET_COUNT];

static float gravity = 900.0f;
static Rectangle dropZone = {300, 0, 200, 40};
static Rectangle replayBtn = {650, 20, 120, 40};

void initGame(void)
{
    ballCount = 0;
    ballsLeft = 10;
    totalScore = 0;

    // Main pegs
    int index = 0;
    int startY = 150;
    int rows = 7;
    int cols = 10;
    int spacingX = 65;
    int spacingY = 60;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (index >= PEG_COUNT) break;
            float offset = (row % 2 == 0) ? 0 : spacingX / 2;

            pegs[index++] = (Peg){
                (Vector2){60 + col * spacingX + offset,
                          startY + row * spacingY},
                9
            };
        }
    }

    // Mini pegs
    for (int i = 0; i < MINI_PEG_COUNT; i++) {
        miniPegs[i] = (Peg){
            (Vector2){180 + i * 45, 120},
            6
        };
    }

    // Buckets
    int scores[BUCKET_COUNT] = {10, 30, 100, 30, 10};
    buckets[0].rect = (Rectangle){0,   540, 180, 60};
    buckets[1].rect = (Rectangle){180, 540, 160, 60};
    buckets[2].rect = (Rectangle){340, 540, 120, 60};
    buckets[3].rect = (Rectangle){460, 540, 160, 60};
    buckets[4].rect = (Rectangle){620, 540, 180, 60};

    for (int i = 0; i < BUCKET_COUNT; i++)
        buckets[i].score = scores[i];
}

void resetGame(void)
{
    initGame();
}

void updateGame(void)
{
    float dt = GetFrameTime();
    Vector2 mouse = GetMousePosition();

    // Replay
    if (CheckCollisionPointRec(mouse, replayBtn) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        resetGame();
    }

    // Drop ball
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        ballsLeft > 0 &&
        CheckCollisionPointRec(mouse, dropZone)) {

        Ball b = {0};
        b.position = mouse;
        b.radius = 9;
        b.active = true;
        b.color = (Color){
            GetRandomValue(50,255),
            GetRandomValue(50,255),
            GetRandomValue(50,255),
            255
        };
        balls[ballCount++] = b;
        ballsLeft--;
    }

    // Update balls
    for (int i = 0; i < ballCount; i++) {
        if (!balls[i].active) continue;

        balls[i].velocity.y += gravity * dt;
        balls[i].position = Vector2Add(
            balls[i].position,
            Vector2Scale(balls[i].velocity, dt)
        );

        // Peg collisions
        for (int p = 0; p < PEG_COUNT; p++) {
            Vector2 diff = Vector2Subtract(balls[i].position, pegs[p].position);
            float dist = Vector2Length(diff);
            float minDist = balls[i].radius + pegs[p].radius;

            if (dist < minDist) {
                Vector2 normal = Vector2Normalize(diff);
                balls[i].position = Vector2Add(
                    pegs[p].position,
                    Vector2Scale(normal, minDist)
                );
                float dot = Vector2DotProduct(balls[i].velocity, normal);
                balls[i].velocity = Vector2Subtract(
                    balls[i].velocity,
                    Vector2Scale(normal, 2 * dot)
                );
                balls[i].velocity = Vector2Scale(balls[i].velocity, 0.8f);
            }
        }

        // Mini pegs
        for (int p = 0; p < MINI_PEG_COUNT; p++) {
            Vector2 diff = Vector2Subtract(balls[i].position, miniPegs[p].position);
            if (Vector2Length(diff) < balls[i].radius + miniPegs[p].radius)
                balls[i].velocity.x += diff.x * 4;
        }

        // Buckets
        for (int b = 0; b < BUCKET_COUNT; b++) {
            if (CheckCollisionCircleRec(
                    balls[i].position,
                    balls[i].radius,
                    buckets[b].rect)) {
                totalScore += buckets[b].score;
                balls[i].active = false;
            }
        }
    }
}

void drawGame(void)
{
    // Drop zone
    DrawRectangleRec(dropZone, Fade(SKYBLUE, 0.4f));
    DrawText("DROP", 370, 10, 20, DARKBLUE);

    // Pegs
    for (int i = 0; i < PEG_COUNT; i++)
        DrawCircleV(pegs[i].position, pegs[i].radius, DARKGRAY);

    for (int i = 0; i < MINI_PEG_COUNT; i++)
        DrawCircleV(miniPegs[i].position, miniPegs[i].radius, GRAY);

    // Buckets
    for (int i = 0; i < BUCKET_COUNT; i++) {
        DrawRectangleRec(buckets[i].rect, LIGHTGRAY);
        DrawText(TextFormat("%d", buckets[i].score),
                 buckets[i].rect.x + 35,
                 buckets[i].rect.y + 20,
                 20, BLACK);
    }

    // Balls
    for (int i = 0; i < ballCount; i++)
        if (balls[i].active)
            DrawCircleV(balls[i].position, balls[i].radius, balls[i].color);

    // UI
    DrawText(TextFormat("Score: %d", totalScore), 20, 20, 30, BLACK);
    DrawText(TextFormat("Balls Left: %d", ballsLeft), 20, 60, 22, BLACK);

    // Replay
    DrawRectangleRec(replayBtn, Fade(LIGHTGRAY, 0.6f));
    DrawText("REPLAY", 665, 30, 20, BLACK);
}
