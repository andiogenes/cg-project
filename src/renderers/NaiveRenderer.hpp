#pragma once

#include "Renderer.hpp"
#include "raylib.h"
#include <vector>
#include <cmath>

class NaiveRenderer : public Renderer {
public:
    NaiveRenderer(int width, int height, int length) : width(width), height(height), length(length) {
        auto perlin = GenImagePerlinNoise(width, height, 10, 10, 1.f);

        model = std::vector<std::vector<std::vector<uint8_t>>>(width);

        for (int x = 0; x < width; x++) {
            auto row = std::vector<std::vector<uint8_t>>(height);

            for (int z = 0; z < height; z++) {
                auto column = std::vector<uint8_t>(length);

                auto c = GetPixelColor((Color *) perlin.data + z * width + x, UNCOMPRESSED_R8G8B8);
                float l = std::round(((float)(c.r)/255.f) * 10);

                std::fill(column.begin() + std::floor(l), column.end(), 1);
                row[z] = column;
            }

            model[x] = row;
        }

        UnloadImage(perlin);

        cube.materials[0].maps[MAP_DIFFUSE].texture = atlas["grass"];
    }

    ~NaiveRenderer() override {
        UnloadModel(cube);
    }

    void render2D() override {}

    void render3D() override {
        for (auto xIt = model.begin(); xIt != model.end(); ++xIt) {
            int i = std::distance(model.begin(), xIt);
            for (auto zIt = xIt->begin(); zIt != xIt->end(); ++zIt) {
                int j = std::distance(xIt->begin(), zIt);
                for (auto yIt = zIt->begin(); yIt != zIt->end(); ++yIt) {
                    int k = std::distance(zIt->begin(), yIt);
                    if (*yIt != 0) {
                        const Vector3 pos = {
                                (float) width / 2 - (float) i,
                                (float) length / 2 - (float) k,
                                (float) height / 2 - (float) j
                        };
                        DrawModel(cube, pos, 1.f, WHITE);
                        DrawCubeWires(pos, 1.f, 1.f, 1.f, BLACK);
                    }
                }
            }
        }
    }

private:
    const int width, height, length;
    std::vector<std::vector<std::vector<uint8_t>>> model;
    Model cube = LoadModelFromMesh(GenMeshCube(1.f, 1.f, 1.f));
};