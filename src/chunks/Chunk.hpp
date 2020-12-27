#pragma once

#include "raylib.h"
#include <array>
#include <vector>

#define NEIGHBOUR_UP 0
#define NEIGHBOUR_DOWN 1
#define NEIGHBOUR_LEFT 2
#define NEIGHBOUR_RIGHT 3
#define NEIGHBOUR_FORWARD 4
#define NEIGHBOUR_BACKWARD 5

template<size_t width, size_t height, size_t length>
struct Chunk {
    void calculateBorder() {
        for (int x = 0; x < width; x++) {
            for (int z = 0; z < height; z++) {
                for (int y = 0; y < length; y++) {
                    if (data[x][z][y] == 0) continue;

                    int neighbours_count = 0;
                    uint8_t faceMask = 0;

                    // TODO: refactor me!

                    if (x - 1 >= 0) {
                        if (data[x - 1][z][y] != 0) neighbours_count++;
                        else faceMask |= 0x1 << 0;
                    } else if (neighbours[NEIGHBOUR_LEFT] != nullptr && neighbours[NEIGHBOUR_LEFT]->data[width - 1][z][y] != 0) {
                        neighbours_count++;
                    } else {
                        faceMask |= 0x1 << 0;
                    }
                    if (x + 1 < width) {
                        if (data[x + 1][z][y] != 0) neighbours_count++;
                        else faceMask |= 0x1 << 1;
                    } else if (neighbours[NEIGHBOUR_RIGHT] != nullptr && neighbours[NEIGHBOUR_RIGHT]->data[0][z][y] != 0) {
                        neighbours_count++;
                    } else {
                        faceMask |= 0x1 << 1;
                    }
                    if (z - 1 >= 0) {
                        if (data[x][z - 1][y] != 0) neighbours_count++;
                        else faceMask |= 0x1 << 2;
                    } else if (neighbours[NEIGHBOUR_FORWARD] != nullptr && neighbours[NEIGHBOUR_FORWARD]->data[x][height - 1][y] != 0) {
                        neighbours_count++;
                    } else {
                        faceMask |= 0x1 << 2;
                    }
                    if (z + 1 < height) {
                        if (data[x][z + 1][y] != 0) neighbours_count++;
                        else faceMask |= 0x1 << 3;
                    } else if (neighbours[NEIGHBOUR_BACKWARD] != nullptr && neighbours[NEIGHBOUR_BACKWARD]->data[x][0][y] != 0) {
                        neighbours_count++;
                    } else {
                        faceMask |= 0x1 << 3;
                    }
                    if (y - 1 >= 0) {
                        if (data[x][z][y - 1] != 0) neighbours_count++;
                        else faceMask |= 0x1 << 4;
                    } else if (neighbours[NEIGHBOUR_UP] != nullptr && neighbours[NEIGHBOUR_UP]->data[x][z][length - 1] != 0) {
                        neighbours_count++;
                    } else {
                        faceMask |= 0x1 << 4;
                    }
                    if (y + 1 < length) {
                        if (data[x][z][y + 1] != 0) neighbours_count++;
                        else faceMask |= 0x1 << 5;
                    } else if (neighbours[NEIGHBOUR_DOWN] != nullptr && neighbours[NEIGHBOUR_DOWN]->data[x][z][0] != 0) {
                        neighbours_count++;
                    } else {
                        faceMask |= 0x1 << 5;
                    }

                    if (neighbours_count < 6) {
                        border.push_back({{(float) x, (float) y, (float) z}, (uint8_t)(data[x][z][y] - 1), faceMask});
                    }
                }
            }
        }
    }

    struct {
        int x;
        int y;
        int z;
    } offset;

    struct BorderInfo {
        Vector3 pos;
        uint8_t tpe;
        uint8_t faceMask;
    };

    std::array<Chunk*, 6> neighbours = { nullptr };
    std::vector<BorderInfo> border;
    std::array<std::array<std::array<uint8_t, length>, height>, width> data = { 0 };
};