#include "libs/terrain.h"
#include <raylib.h>
#include <cmath>
#include <random>
#include <vector>

std::random_device rd;
std::default_random_engine prngDevice(rd());
std::uniform_int_distribution<int> distribution(0, 1000);
Chunk::offsetX = distribution(prngDevice);
Chunk::offsetY = distribution(prngDevice);

Chunk::Chunk(int x, int y) {
    Vector3 terrainPos = {Chunk::chunkSize*x, -Chunk::chunkHeight/2, Chunk::chunkSize*y};

    // The chunk itself
    Image perlinNoise = GenImagePerlinNoise(64, 64, offsetX + (chunkCoordX*chunkSize), offsetY + (chunkCoordY*chunkSize), 2.0f);
    Mesh terrainMesh = GenMeshHeightmap(perlinNoise, terrainSize);
    Model terrainModel = LoadModelFromMesh(terrainMesh);
    terrainModel.materials[0].shader = terrainShader;
    UnloadImage(perlinNoise);
}

void Chunk::GenerateChunks() {
    // The custom terrain shader (lighting + heigh-based color)
    Chunk::terranShader = LoadShader("shaders/terrain.vs", "shaders/terrain.fs");
    terrainShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(terrainShader, "viewPos");
    
    for (int x = 0; x < Chunk::chunkNum; x++) {
        for (int y = 0; y < Chunk::chunkNum; y++) {
            Chunk::Chunk(x, y)
        }
    }
}

void Chunk::DrawChunks() {

}

void Chunk::UnloadChunks() {

}