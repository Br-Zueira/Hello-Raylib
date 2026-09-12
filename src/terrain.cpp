#include "libs/terrain.h"
#include <raylib.h>
#include <cmath>
#include <random>
#include <vector>

class Chunk {
    public:
        Chunk(int x, int y); // X and Y are positions in chunk grid
        static void GenerateChunks(); // Generates all chunks at once
        static void DrawChunks(); // Draws all chunks at once
        static void UnloadChunks(); // Unloads shader and all chunk models
        static Shader terrainShader;
        Model chunkModel;
        Vector3 chunkPosition;
    private:
        static constexpr int chunkNum = 5;
        static constexpr int chunkSize = 64; // Only X/Z (X/Y in grid)
        static constexpr Vector3 chunkSizeVec3 = {chunkSize, 25, chunkSize}; // X, Y and Z (chunkSize, height, chunkSize)
        static std::vector<Chunk> generatedChunks; // Storaged so it can be easily drawed and updated later
        static std::random_device rd;
        static std::default_random_engine prngDevice;
        static std::uniform_int_distribution<int> distribution;
        static int offsetX;
        static int offsetY;
}

function Chunk::Chunk(int x, int y) {

}

function Chunk::GenerateChunks() {

}

function Chunk::DrawChunks() {

}

function Chunk::UnloadChunks() {

}