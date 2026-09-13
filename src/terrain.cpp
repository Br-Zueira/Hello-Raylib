#include "libs/terrain.h"
#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include <random>
#include <vector>
#include <algorithm>
#include "libs/rlights.h"

#include "iostream"

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
    chunkPosition = {Chunk::chunkSize * x, -Chunk::chunkHeight / 2, Chunk::chunkSize * y};

    // Perlin noise and chunk mesh generation
    float pnSize = chunkSize + 1; // Corrects gap between chunks
    Image perlinNoise = GenImagePerlinNoise(pnSize, pnSize, offsetX + (x * chunkSize), offsetY + (y * chunkSize), 1.25f);
    Mesh chunkMesh = GenMeshHeightmap(perlinNoise, chunkSizeVec3);

    // Generation of the chunk model itself
    chunkModel = LoadModelFromMesh(chunkMesh);
    chunkModel.materials[0].shader = terrainShader;

    // Unloads the Perlin Noise image to free RAM and avoid memory leaks
    UnloadImage(perlinNoise);
}

// Chunk deconstructor
Chunk::~Chunk() {
    if (IsModelValid(chunkModel)) {
        UnloadModel(chunkModel);
    }
}

void Chunk::Init() {
    // The custom terrain shader (lighting + heigh-based color)
    Chunk::terrainShader = LoadShader("shaders/terrain.vs", "shaders/terrain.fs");
    terrainShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(terrainShader, "viewPos");
}

// Creates all chunks at once
void Chunk::UpdateChunks(Vector3 cameraPos) {
    // Converts the camera 3D position to the chunk grid position it belongs to
    int cameraGridPosX = static_cast<int>(std::floor(cameraPos.x / Chunk::chunkSize + EPSILON));
    int cameraGridPosY = static_cast<int>(std::floor(cameraPos.z / Chunk::chunkSize + EPSILON));
    Vector2 cameraGridPos = {static_cast<float>(cameraGridPosX), static_cast<float>(cameraGridPosY)};

    std::vector<Vector2> requestedBatch;

    // Calculates the chunks that should exist
    int radius = static_cast<int>(Chunk::renderDistance/2);
    for (int x = cameraGridPosX - radius; x <= cameraGridPosX + radius; x++) {
        for (int y = cameraGridPosY - radius; y <= cameraGridPosY + radius; y++) {
            requestedBatch.emplace_back(Vector2{static_cast<float>(x), static_cast<float>(y)});
        }
    }

    std::vector<Vector2> redundantChunks;

    // Checks existing chunks to see if they should be kept or thrown into oblivion forever
    for (auto chunkInstance = Chunk::generatedChunks.begin(); chunkInstance != Chunk::generatedChunks.end(); ) {
        int chunkGridPosX = static_cast<int>(std::floor(chunkInstance->chunkPosition.x / Chunk::chunkSize + EPSILON));
        int chunkGridPosY = static_cast<int>(std::floor(chunkInstance->chunkPosition.z / Chunk::chunkSize + EPSILON));
        Vector2 chunkGridPos = {static_cast<float>(chunkGridPosX), static_cast<float>(chunkGridPosY)};
        if (std::find(requestedBatch.begin(), requestedBatch.end(), chunkGridPos) != requestedBatch.end()) {
            // Tells program that these don't need to be created
            redundantChunks.push_back(chunkGridPos);
            ++chunkInstance;
        } else {
            // Remove out of bounds chunks
            chunkInstance = Chunk::generatedChunks.erase(chunkInstance);
        }
    }

    // Spawn requested chunks that don't exist yet
    for (auto& chunkGridPos : requestedBatch) {
        if (std::find(redundantChunks.begin(), redundantChunks.end(), chunkGridPos) == redundantChunks.end()) {
            Chunk::generatedChunks.emplace_back(chunkGridPos.x, chunkGridPos.y);
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
        if (IsModelValid(chunkInstance.chunkModel)) {
            UnloadModel(chunkInstance.chunkModel);
        }
    }
    UnloadShader(Chunk::terrainShader);
}