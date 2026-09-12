#include "libs/terrain.h"
#include <raylib.h>
#include <cmath>
#include <random>
#include <vector>
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

// Chunk constructors
Chunk::Chunk(int x, int y) {
    // Position of the said chunk
    chunkPosition = {Chunk::chunkSize*x, -Chunk::chunkHeight/2, Chunk::chunkSize*y};

    // Perlin noise and chunk mesh generation
    Image perlinNoise = GenImagePerlinNoise(chunkSize, chunkSize, offsetX + (x*chunkSize), offsetY + (y*chunkSize), 2.0f);
    Mesh chunkMesh = GenMeshHeightmap(perlinNoise, chunkSizeVec3);

    // Generation of the chunk model itself
    chunkModel = LoadModelFromMesh(chunkMesh);
    chunkModel.materials[0].shader = terrainShader;

    // Unloads the Perlin Noise image to free RAM and avoid memory leaks
    UnloadImage(perlinNoise);
}

// Creates all chunks at once
void Chunk::GenerateChunks() {
    // The custom terrain shader (lighting + heigh-based color)
    Chunk::terrainShader = LoadShader("shaders/terrain.vs", "shaders/terrain.fs");
    terrainShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(terrainShader, "viewPos");
    
    // Generates each chunk, along with its respective coordinate in the grid
    for (int x = 0; x < Chunk::chunkNum; x++) {
        for (int y = 0; y < Chunk::chunkNum; y++) {
            Chunk::generatedChunks.emplace_back(x, y);
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