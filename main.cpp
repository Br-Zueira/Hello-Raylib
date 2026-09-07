#include <raylib.h>
#include <cmath>
#include <iostream>

int main() {
    // Creates a brand new window
    int vw = 800;
    int vy = 450;
    InitWindow(vw, vy, "Hello Raylib");
    SetTargetFPS(60);

    // Settings for rendering a triangle
    float sideSize = 200.0f;

    // Trigonometry nerdy moment: sqrt(3)/2 is tangent of 30°, this is used to make the triangle equilateral
    float height = sideSize * (sqrt(3.0f) / 2.0f);

    // Setting up a color for background (white)
    Color white = {255, 255, 255, 0};

    // Setting up each vertex of the triangle (centralized and equilateral)
    Vector2 dot1 = {vw / 2.0f, vy / 3.0f};
    Vector2 dot2 = {vw / 2.0f - (sideSize / 2.0f), dot1.y + height};
    Vector2 dot3 = {vw / 2.0f + (sideSize / 2.0f), dot1.y + height};

    // Setting up some text to render
    Font stdfont = GetFontDefault(); // Standard Raylib font
    const char *motto = "Libertas quæ sera tamen"; // Minas Gerais flag moto ("Freedom, even if late")
    int fontSize = 20; // Arbitrary font size
    float textWidth = MeasureText(motto, fontSize); // Width of text, used to centralize the text
    float textPosX = dot1.x - textWidth/2; // Centered text
    float textPosY = dot1.y + height + fontSize; // Text right below the triangle
    Color black = {0, 0, 0, 255};

    // Mutable color channel for triangle
    int redChannel = 0;

    // Amount per second to change the red channel
    float shiftPerSec = 200;

    // Project Loop
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Dynamic color
        if (IsKeyDown(KEY_A)) { redChannel-= shiftPerSec*dt; } 
        else if (IsKeyDown(KEY_D)) { redChannel+= shiftPerSec*dt; }
        
        // Variable wrapping
        if (redChannel > 255) { redChannel = 255; }
        else if (redChannel < 0) { redChannel = 0; }

        // Dynamic color for triangle
        Color triangleColor = {redChannel, 0, 0, 255};

        BeginDrawing();

        ClearBackground(white);
        DrawTriangle(dot1, dot2, dot3, triangleColor);
        DrawText(motto, textPosX, textPosY, fontSize, black);

        EndDrawing();
    }
 
    // Properly finishes session
    CloseWindow();
    return 0;
}