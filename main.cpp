#include <raylib.h>
#include <cmath>
#include <iostream>

int main() {
    // Creates a brand new window
    int vw = 800;
    int vy = 450;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(vw, vy, "Hello Raylib");
    SetTargetFPS(60);

    // Disable the exit key (so ESC is free for me to use :) )
    SetExitKey(KEY_NULL);

    // Settings for rendering a triangle
    float sideSize = 200.0f;

    // Trigonometry nerdy moment: sqrt(3)/2 is tangent of 30°, this is used to make the triangle equilateral
    float height = sideSize * (sqrt(3.0f) / 2.0f);

    // Setting up a color for background (white)
    Color white = {255, 255, 255, 255};

    // Setting up some text to render
    Font stdfont = GetFontDefault(); // Standard Raylib font
    const char *motto = "Libertas quæ sera tamen"; // Minas Gerais flag moto ("Freedom, even if late")
    int fontSize = 20; // Arbitrary font size
    float textWidth = MeasureText(motto, fontSize); // Width of text, used to centralize the text
    Color black = {0, 0, 0, 255};

    // Project Loop
    while (!WindowShouldClose()) {
        // Toggles cursor (Enter locks it and Esc unlocks it)
        if (IsKeyPressed(KEY_ESCAPE)) {
            EnableCursor();
        }
        if (IsKeyPressed(KEY_ENTER)) {
            DisableCursor();
        }

        // Some useful variables :)
        vw = GetScreenWidth();
        vy = GetScreenHeight();
        float dt = GetFrameTime();

        // Setting up each vertex of the triangle (centralized and equilateral)
        Vector2 dot1 = {vw / 2.0f, vy / 3.0f};
        Vector2 dot2 = {vw / 2.0f - (sideSize / 2.0f), dot1.y + height};
        Vector2 dot3 = {vw / 2.0f + (sideSize / 2.0f), dot1.y + height};

        // Dynamic color based on sine waves
        unsigned char channel = 255*((sinf(GetTime())+1)/2);

        // Dynamic color for triangle
        Color triangleColor = {255, channel, channel, 255};

        BeginDrawing();

        ClearBackground(black);
        DrawTriangle(dot1, dot2, dot3, triangleColor);
        DrawText(motto, 10, 10, fontSize, white); // Draws text at top-left with a margin of 10px

        EndDrawing();
    }
 
    // Properly finishes session
    CloseWindow();
    return 0;
}