#ifndef TERRAIN_H
#define TERRAIN_H

#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include <random>
#include <vector>
#include <algorithm>
#include "rlights.h"

class Chunk {
    public:
        Chunk(int x, int y); // X and Y are positions in chunk grid
        ~Chunk();
        static void Init();
        static void UpdateChunks(Vector3 cameraPos);
        static void DrawChunks(float cameraPos[3], Light light); // Draws all chunks at once
        static void UnloadChunks(); // Unloads shader and all chunk models
        static Shader terrainShader;
        Model chunkModel;
        Vector3 chunkPosition;
    private:
        static std::vector<Chunk> generatedChunks;
        static constexpr float chunkSize = 64.0f; // Only X/Z (X/Y in grid)
        static constexpr float chunkHeight = 25.0f;
        static constexpr Vector3 chunkSizeVec3 = {chunkSize, chunkHeight, chunkSize}; // X, Y and Z (chunkSize, height, chunkSize)
        static int offsetX;
        static int offsetY;
        static constexpr float renderDistance = chunkSize*5;
};

#endif