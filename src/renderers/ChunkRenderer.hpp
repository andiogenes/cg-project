#pragma once

#include "Renderer.hpp"
#include "../chunks/Chunk.hpp"
#include "raylib.h"
#include <vector>
#include <cmath>
#include <iostream>

const std::array<std::string, 3> textureAliases = {"grass", "dirt", "sand"};

class ChunkRenderer : public Renderer {
public:
    ChunkRenderer(int width, int height, int length) : width(width), height(height), length(length) {
        auto xChunks = std::max(1, width / CHUNK_SIZE);
        auto yChunks = std::max(1, length / CHUNK_SIZE);
        auto zChunks = std::max(1, height / CHUNK_SIZE);

        auto chunksCount = xChunks * zChunks * yChunks;

        chunks.resize(chunksCount);

        for (int i = 0; i < xChunks; i++) {
            for (int j = 0; j < zChunks; j++) {
                for (int k = 0; k < yChunks; k++) {
                    auto current = (zChunks * k + j) * xChunks + i;

                    chunks[current].offset = {i, k, j};

                    if (k - 1 >= 0) chunks[current].neighbours[NEIGHBOUR_UP] = &chunks[(zChunks * (k - 1) + j) * xChunks + i];
                    if (k + 1 < yChunks) chunks[current].neighbours[NEIGHBOUR_DOWN] = &chunks[(zChunks * (k + 1) + j) * xChunks + i];
                    if (i - 1 >= 0) chunks[current].neighbours[NEIGHBOUR_LEFT] = &chunks[(zChunks * k + j) * xChunks + i - 1];
                    if (i + 1 < xChunks) chunks[current].neighbours[NEIGHBOUR_RIGHT] = &chunks[(zChunks * k + j) * xChunks + i + 1];
                    if (j - 1 >= 0) chunks[current].neighbours[NEIGHBOUR_FORWARD] = &chunks[(zChunks * k + (j - 1)) * xChunks + i];
                    if (j + 1 < zChunks) chunks[current].neighbours[NEIGHBOUR_BACKWARD] = &chunks[(zChunks * k + (j + 1)) * xChunks + i];
                }
            }
        }

        for (auto const &chunk: chunks) {
            std::cout << &chunk << ": ";
            for (auto const &n : chunk.neighbours) {
                std::cout << n << " ";
            }
            std::cout << std::endl;
        }

        auto perlin = GenImagePerlinNoise(width, height, 10, 10, 1.f);

        for (int x = 0; x < width; x++) {
            int i = x / CHUNK_SIZE;

            for (int z = 0; z < height; z++) {
                int j = z / CHUNK_SIZE;

                auto c = GetPixelColor((Color *) perlin.data + z * width + x, UNCOMPRESSED_R8G8B8);
                int l = std::floor(((float) (c.r) / 255.f) * 10);

                int k = l / CHUNK_SIZE;

                auto &data = chunks[(zChunks * k + j) * xChunks + i].data;

                data[x % CHUNK_SIZE][z % CHUNK_SIZE][(length - 1 - l) % CHUNK_SIZE] = l > length / 4 ? 1 : 3;
                std::fill(data[x % CHUNK_SIZE][z % CHUNK_SIZE].begin() + (length - l) % CHUNK_SIZE, data[x % CHUNK_SIZE][z % CHUNK_SIZE].end(), 2);
            }
        }

        UnloadImage(perlin);

        for (auto &chunk : chunks) {
            chunk.calculateBorder();
        }

        plane = LoadModelFromMesh(GenMeshPlane(1.f, 1.f, 1, 1));
    }

    ~ChunkRenderer() override {
        UnloadModel(plane);
    }

    void render2D() override {}

    void render3D(const Camera3D &camera) override {
        for (auto const &chunk : chunks) {
            for (const auto & it : chunk.border) {
                const Vector3 pos = {
                        (float) width / 2 - it.pos.x - (float)chunk.offset.x * CHUNK_SIZE,
                        (float) length / 2 - it.pos.y - (float)chunk.offset.y * CHUNK_SIZE,
                        (float) height / 2 - it.pos.z - (float)chunk.offset.z * CHUNK_SIZE
                };
                plane.materials[0].maps[MAP_DIFFUSE].texture = atlas[textureAliases[it.tpe]];
                if ((it.faceMask >> 0) & 0x1)
                    DrawModelEx(plane, {pos.x + 0.5f, pos.y, pos.z}, {0.f, 0.f, -1.f}, 90.f, {1.f, 1.f, 1.f}, LIGHTGRAY);
                if ((it.faceMask >> 1) & 0x1)
                   DrawModelEx(plane, {pos.x - 0.5f, pos.y, pos.z}, {0.f, 0.f, 1.f}, 90.f, {1.f, 1.f, 1.f}, GRAY);
                if ((it.faceMask >> 2) & 0x1)
                    DrawModelEx(plane, {pos.x, pos.y, pos.z + 0.5f}, {1.f, 0.f, 0.f}, 90.f, {1.f, 1.f, 1.f}, LIGHTGRAY);
                if ((it.faceMask >> 3) & 0x1)
                    DrawModelEx(plane, {pos.x, pos.y, pos.z - 0.5f}, {-1.f, 0.f, 0.f}, 90.f, {1.f, 1.f, 1.f}, GRAY);
                if ((it.faceMask >> 4) & 0x1)
                    DrawModel(plane, {pos.x, pos.y + 0.5f, pos.z}, 1.f, WHITE);
                if ((it.faceMask >> 5) & 0x1)
                    DrawModelEx(plane, {pos.x, pos.y - 0.5f, pos.z}, {1.f, 0.f, 0.f}, 180.f, {1.f, 1.f, 1.f}, DARKGRAY);
            }
        }
    }

private:
    const int width, height, length;

    static const int CHUNK_SIZE = 16;
    std::vector<Chunk<CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE>> chunks;

    Model plane = {};
};
