#include <raylib.h>
#include <cmath>
#include <string>
#include <random>

#define RLIGHTS_IMPLEMENTATION
#include "libs/rlights.h"
#undef RLIGHTS_IMPLEMENTATION

#include "libs/terrain.h"

int main() {
    // Creates a brand new window
    int vw = 800;
    int vy = 450;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(vw, vy, "Hello Raylib");
    SetTargetFPS(60);

    // Disable the exit key (so ESC is free for me to use :) )
    SetExitKey(KEY_NULL);

    // Setting up lighting
    Shader lightingShader = LoadShader("example-shaders/lighting.vs", "example-shaders/lighting.fs");
    lightingShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(lightingShader, "viewPos");

    Light light = CreateLight(LIGHT_POINT, (Vector3){0.0f, 10.0f, 0.0f}, (Vector3){0}, WHITE, lightingShader);

    // Settings for rendering a triangle
    float sideSize = 8.0f;

    // Trigonometry nerdy moment: sqrt(3)/2 is tangent of 30°, this is used to make the triangle equilateral
    float height = sideSize * (sqrt(3.0f) / 2.0f);

    // Setting up some text to render
    Font stdfont = GetFontDefault(); // Standard Raylib font
    const char *motto = "Libertas quæ sera tamen"; // Minas Gerais flag moto ("Freedom, even if late")
    int fontSize = 20; // Arbitrary font size
    float textWidth = MeasureText(motto, fontSize); // Width of text, used to centralize the text

    // Creating a new camera
    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 2.0f, 4.0f};
    camera.target = (Vector3){0.0f, 2.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Setting up each vertex of the triangle
    Vector3 dot1 = {0.0f, height + 12.0f, 0.0f};
    Vector3 dot2 = {dot1.x - sideSize/2.0f, dot1.y - height, dot1.z};
    Vector3 dot3 = {dot1.x + sideSize/2.0f, dot1.y - height, dot1.z};

    // Setting up sphere params
    Vector3 spherePos = {10.0f, 20.0f, 10.0f};
    Mesh sphereMesh = GenMeshSphere(4.0f, 64, 64);
    Model sphereModel = LoadModelFromMesh(sphereMesh);
    Texture2D brickTexture = LoadTexture("assets/textures/bricks.png");
    GenTextureMipmaps(&brickTexture); // Basically "detail levels"
    SetTextureFilter(brickTexture, TEXTURE_FILTER_TRILINEAR); // Smoother transition between mipmaps
    sphereModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = brickTexture;
    sphereModel.materials[0].shader = lightingShader;

    // Setting up cube params
    Vector3 cubePos = {-5.0f, 15.0f, 8.0f};
    Mesh cubeMesh = GenMeshCube(4.0f, 5.0f, 6.0f);
    Model cubeModel = LoadModelFromMesh(cubeMesh);
    Color green = {0, 255, 0, 255};
    Image greenImg = GenImageColor(1, 1, green);
    Texture2D greenTexture = LoadTextureFromImage(greenImg); // No need for mipmaps because it's already only 1 pixel
    cubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = greenTexture;
    cubeModel.materials[0].shader = lightingShader;
    UnloadImage(greenImg);

    // Setting up wire cylinder params
    float cylinderRadius = 5.0f;
    float cylinderHeight = 15.0f;
    int cylinderSlices = 20;
    Vector3 cylinderPos = {25.0f, 0.0f, 3.0f};
    Color yellow = {255, 255, 0, 255};

    // Setting up bust params
    Vector3 bustPos = {8.0f, 15.0f, -5.0f};
    Model bustModel = LoadModel("assets/marble_bust_01_4k.gltf");
    GenTextureMipmaps(&bustModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
    SetTextureFilter(bustModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture, TEXTURE_FILTER_TRILINEAR);
    for (int i = 0; i < bustModel.materialCount; i++) {
        bustModel.materials[i].shader = lightingShader;
    }

    // Useful to make a toggle option for updating camera
    bool updateCam = true;

    Chunk::Init();

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
        if (updateCam) {
            if (IsKeyDown(KEY_P)) {
                camera.projection = CAMERA_ORTHOGRAPHIC;
            } else {
                camera.projection = CAMERA_PERSPECTIVE;
            }
        }

        // Delta time (useful for physics)
        float dt = GetFrameTime();

        // Updates camera rotation and position
        if (IsKeyPressed(KEY_C)) { updateCam = !updateCam; } // Toggles camera update on and off
        if (updateCam) { UpdateCamera(&camera, CAMERA_FREE); }

        // Dynamic color based on sine waves
        unsigned char channel = 255*((sinf(GetTime())+1)/2);
        Color triangleColor = {255, channel, channel, 255};

        // Changes camera FOV based on mouse wheel vertical delta
        camera.fovy -= GetMouseWheelMoveV().y*2.0f;

        // Makes light teleport to camera
        if (IsKeyDown(KEY_L)) { light.position = camera.position; }

        Chunk::UpdateChunks(camera.position);
        
        BeginDrawing();
        
        ClearBackground(SKYBLUE);
        
        // Updates lighting shader values, specially picking up camera position
        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(lightingShader, lightingShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        UpdateLightValues(lightingShader, light);

        BeginMode3D(camera);
        if (IsKeyDown(KEY_G)) { DrawGrid(100, 5); }
        DrawTriangle3D(dot1, dot2, dot3, triangleColor);
        DrawModel(sphereModel, spherePos, 1.0f, WHITE);
        DrawModel(cubeModel, cubePos, 1.0f, WHITE);
        DrawCylinderWires(cylinderPos, cylinderRadius, cylinderRadius, cylinderHeight, cylinderSlices, yellow);
        DrawModel(bustModel, bustPos, 10.0f, WHITE);
        Chunk::DrawChunks(cameraPos, light);
        EndMode3D();

        // Draws motto at top-left with a margin of 10px
        DrawText(motto, 10, 10, fontSize, WHITE);

        // FOV status
        std::string fovyInfo = "FOV: " + std::to_string(static_cast<int>(camera.fovy)); // Message (example: "FOV: 60"). Casts to int first to cut out decimal part (otherwise it'll print like "60.00000")
        DrawText(fovyInfo.c_str(), 10, vy-10-fontSize, fontSize, WHITE); // Prints the FOV status at bottom-left

        DrawFPS(vw-80, 10);

        EndDrawing();
    }

    // Avoids memory leaks
    UnloadModel(sphereModel);
    UnloadModel(cubeModel);
    UnloadModel(bustModel);
    UnloadTexture(brickTexture);
    UnloadTexture(greenTexture);
    UnloadShader(lightingShader);
    Chunk::UnloadChunks();

    // Closes the window after program shut down
    CloseWindow();
    return 0;
}