#ifndef TERRAIN_H
#define TERRAIN_H

#include <raylib.h>
#include <cmath>
#include <random>
#include <vector>
#include "rlights.h"

class Chunk {
    public:
        Chunk(int x, int y); // X and Y are positions in chunk grid
        static void GenerateChunks(); // Generates all chunks at once
        static void DrawChunks(float cameraPos[3], Light light); // Draws all chunks at once
        static void UnloadChunks(); // Unloads shader and all chunk models
        static Shader terrainShader;
        Model chunkModel;
        Vector3 chunkPosition;
    private:
        static std::vector<Chunk> generatedChunks;
        static constexpr int chunkNum = 5;
        static constexpr float chunkSize = 64.0f; // Only X/Z (X/Y in grid)
        static constexpr float chunkHeight = 25.0f;
        static constexpr Vector3 chunkSizeVec3 = {chunkSize, chunkHeight, chunkSize}; // X, Y and Z (chunkSize, height, chunkSize)
        static int offsetX;
        static int offsetY;
};

#endif