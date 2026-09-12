#include "libs/terrain.h"
#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include <random>
#include <vector>
#include <algorithm>
#include "libs/rlights.h"

// Setting up PRNG device and
std::random_device rd;
std::default_random_engine prngDevice(rd());
std::uniform_int_distribution<int> distribution(0, 1000);

// Setting up the Perlin Noise "seed"
int Chunk::offsetX = distribution(prngDevice);
int Chunk::offsetY = distribution(prngDevice);

// Setting up Chunk manager static parameters
Shader Chunk::terrainShader;
std::vector<Chunk> Chunk::generatedChunks;

// Chunk constructor
Chunk::Chunk(int x, int y) {
    // Position of the said chunk
    chunkPosition = {Chunk::chunkSize*x, -Chunk::chunkHeight/2, Chunk::chunkSize*y};

    // Perlin noise and chunk mesh generation
    float pnSize = chunkSize + 1; // Corrects gap between chunks
    Image perlinNoise = GenImagePerlinNoise(pnSize, pnSize, offsetX + (x*chunkSize), offsetY + (y*chunkSize), 1.25f);
    Mesh chunkMesh = GenMeshHeightmap(perlinNoise, chunkSizeVec3);

    // Generation of the chunk model itself
    chunkModel = LoadModelFromMesh(chunkMesh);
    chunkModel.materials[0].shader = terrainShader;

    // Unloads the Perlin Noise image to free RAM and avoid memory leaks
    UnloadImage(perlinNoise);
}

// Chunk deconstructor
Chunk::~Chunk() {
    UnloadModel(chunkModel);
}

void Chunk::Init() {
    // The custom terrain shader (lighting + heigh-based color)
    Chunk::terrainShader = LoadShader("shaders/terrain.vs", "shaders/terrain.fs");
    terrainShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(terrainShader, "viewPos");
}

// Creates all chunks at once
void Chunk::UpdateChunks(Vector3 cameraPos) {
    // Converts the camera 3D position to the chunk grid position it belongs to
    int cameraGridPosX = static_cast<int>(std::floor(cameraPos.x/Chunk::chunkSize));
    int cameraGridPosY = static_cast<int>(std::floor(cameraPos.z/Chunk::chunkSize));
    Vector2 cameraGridPos = {cameraGridPosX, cameraGridPosY};

    std::vector<Vector2> requestedBatch;

    // Updates chunks, along with its respective coordinate in the grid
    int radius = static_cast<int>(Chunk::renderDistance/2);
    for (int x = cameraGridPosX - radius; x <= cameraGridPosX + radius; x++) {
        for (int y = cameraGridPosY - radius; y <= cameraGridPosY + radius; y++) {
            Chunk::chunkBrequestedBatchatch.emplace_back(Vector2{x, y});
        }
    }

    std::vector<Vector2> redundantChunks;

    for (auto& chunkInstance : Chunk::generatedChunks) {
        int chunkGridPosX = static_cast<int>(std::floor(chunkInstance.chunkPosition.x/Chunk::chunkSize));
        int chunkGridPosY = static_cast<int>(std::floor(chunkInstance.chunkPosition.z/Chunk::chunkSize));
        Vector2 chunkGridPos = {chunkGridPosX, chunkGridPosY};
        if (std::ranges::contains(requestedBatch, chunkGridPos)) {
            redundantChunks.push_back(chunkGridPos);
        } else {
            
        }
    }
}

// Draw every generated chunk
void Chunk::DrawChunks(float cameraPos[3], Light light) {
    // Updates shader
    SetShaderValue(Chunk::terrainShader, Chunk::terrainShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
    UpdateLightValues(Chunk::terrainShader, light);

    // Draw each chunk
    for (auto& chunkInstance : Chunk::generatedChunks) {
        DrawModel(chunkInstance.chunkModel, chunkInstance.chunkPosition, 1.0f, WHITE);
    }
}

// Unloads chunk model and shader to free RAM and avoid memory leaks
void Chunk::UnloadChunks() {
    for (auto& chunkInstance : Chunk::generatedChunks) {
        UnloadModel(chunkInstance.chunkModel);
    }
    UnloadShader(Chunk::terrainShader);
}