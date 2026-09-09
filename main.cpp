#include <raylib.h>
#include <cmath>
#include <iostream>
#include <string>

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
    float sideSize = 8.0f;

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

    // Creating a new camera
    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 2.0f, 4.0f};
    camera.target = (Vector3){0.0f, 2.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Setting up each vertex of the triangle
    Vector3 dot1 = {0.0f, height, 0.0f};
    Vector3 dot2 = {dot1.x - sideSize/2.0f, dot1.y - height, dot1.z};
    Vector3 dot3 = {dot1.x + sideSize/2.0f, dot1.y - height, dot1.z};

    // Setting up sphere params
    Vector3 spherePos = {10.0f, 10.0f, 10.0f};
    float sphereRadius = 4;
    Color blue = {0, 0, 255, 255};

    // Setting up cobe params
    Vector3 cubePos = {-5.0f, 5.0f, 8.0f};
    Color green = {0, 255, 0, 255};

    // Project Loop
    while (!WindowShouldClose()) {
        // Useful data for UI and 2D elements
        vw = GetScreenWidth();
        vy = GetScreenHeight();

        // Toggles cursor (Enter locks it and Esc unlocks it)
        if (IsKeyPressed(KEY_ESCAPE)) {
            EnableCursor();
        }
        if (IsKeyPressed(KEY_ENTER)) {
            DisableCursor();
        }

        // Makes it possible to analise both types of views (perspective is realistic, orthographic seems to be like globe-to-map projection or 3D-2D slicing)
        if (IsKeyDown(KEY_P)) {
            camera.projection = CAMERA_ORTHOGRAPHIC;
        } else {
            camera.projection = CAMERA_PERSPECTIVE;
        }

        // Delta time (useful for physics)
        float dt = GetFrameTime();

        // Updates camera rotation and position
        UpdateCamera(&camera, CAMERA_FREE);

        // Dynamic color based on sine waves
        unsigned char channel = 255*((sinf(GetTime())+1)/2);
        Color triangleColor = {255, channel, channel, 255};

        // Changes camera FOV based on mouse wheel vertical delta
        camera.fovy -= GetMouseWheelMoveV().y*2.0f;

        BeginDrawing();

        ClearBackground(black);

        BeginMode3D(camera);
        DrawGrid(100, 5);
        DrawTriangle3D(dot1, dot2, dot3, triangleColor);
        DrawSphere(spherePos, sphereRadius, blue);
        DrawCube(cubePos, 4, 5, 6, green);
        EndMode3D();

        // Draws motto at top-left with a margin of 10px
        DrawText(motto, 10, 10, fontSize, white);

        // FOV status
        std::string fovyInfo = "FOV: " + std::to_string(static_cast<int>(camera.fovy)); // Message (example: "FOV: 60"). Casts to int first to cut out decimal part (otherwise it'll print like "60.00000")
        DrawText(fovyInfo.c_str(), 10, vy-10-fontSize, fontSize, white); // Prints the FOV status at bottom-left

        EndDrawing();
    }

    // Properly finishes session
    CloseWindow();
    return 0;
}