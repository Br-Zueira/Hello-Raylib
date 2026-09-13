#ifndef TERRAIN_H
#define TERRAIN_H

#include <raylib.h>
#include <vector>
#include "rlights.h"

#define EPSILON 0.0000001

class Chunk {
    public:
        // Basic functions
        Chunk(int x, int y); // X and Y are positions in chunk grid
        ~Chunk();

        // Avoid shallow duplication
        Chunk(const Chunk&) = delete;
        Chunk& operator=(const Chunk&) = delete;

        // Enable move constructor and move assignment for safe vector reallocation
        Chunk(Chunk&& other) noexcept 
            : chunkPosition(other.chunkPosition), chunkModel(other.chunkModel) {
            other.chunkModel = {0}; // Nullify source so its destructor doesn't free it
        }

        Chunk& operator=(Chunk&& other) noexcept {
            if (this != &other) {
                if (IsModelValid(chunkModel)) UnloadModel(chunkModel);
                chunkPosition = other.chunkPosition;
                chunkModel = other.chunkModel;
                other.chunkModel = {0};
            }
            return *this;
        }

        // Managers
        static void Init();
        static void UpdateChunks(Vector3 cameraPos);
        static void DrawChunks(float cameraPos[3], Light light); // Draws all chunks at once
        static void UnloadChunks(); // Unloads shader and all chunk models
        static Shader terrainShader;

        // Instance specific properties
        Model chunkModel;
        Vector3 chunkPosition;
    private:
        // Required data to generate chunks
        static std::vector<Chunk> generatedChunks;
        static constexpr float chunkSize = 64.0f; // Only X/Z (X/Y in grid)
        static constexpr float chunkHeight = 25.0f;
        static constexpr Vector3 chunkSizeVec3 = {chunkSize, chunkHeight, chunkSize}; // X, Y and Z (chunkSize, height, chunkSize)
        static int offsetX;
        static int offsetY;
        static constexpr float renderDistance = 5;
};

#endif