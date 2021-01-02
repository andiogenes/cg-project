/*
 * Anton Zavyalov, Altai STU, 2020.
 */

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

/**
 * Блок пространственной воксельной информации
 * @tparam width Ширина блока
 * @tparam height Длина блока
 * @tparam length Высота блока
 */
template<size_t width, size_t height, size_t length>
struct Chunk {
    /**
     * Вычисляет трехмерную границу блока. Информация о границе записывается в Chunk.border.
     *
     * @param worldWidth Ширина полного воксельного объекта
     * @param worldHeight Длина полного воксельного объекта
     * @param worldLength Высота полного воксельного объекта
     */
    void calculateBorder(int worldWidth, int worldHeight, int worldLength) {
        for (int x = 0; x < width; x++) {
            for (int z = 0; z < height; z++) {
                for (int y = 0; y < length; y++) {
                    // Пропуск пустых ячеек
                    if (data[x][z][y] == 0) continue;

                    int neighbours_count = 0;
                    uint8_t faceMask = 0;

                    // Принцип определения границы:
                    // 1. Определить, ячейка внутренняя или внешняя
                    // 2. Если внешняя, уточнить видимые грани ячейки
                    // 3. Зафиксировать информацию о граничной ячейке

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
                        const Vector3 pos = {
                                (float) worldWidth / 2 - x - offset.x,
                                (float) worldLength / 2 - y - offset.y,
                                (float) worldHeight / 2 - z - offset.z
                        };
                        border.push_back({pos, (uint8_t)(data[x][z][y] - 1), faceMask});
                    }
                }
            }
        }
    }

    /**
     * Смещение блока относительно начала координат.
     */
    Vector3 offset;

    /**
     * Информация о граничной ячейке.
     */
    struct BorderInfo {
        /**
         * Позиция.
         */
        Vector3 pos;

        /**
         * Тип (номер текстур).
         */
        uint8_t tpe;

        /**
         * Битовая маска видимых вершин.
         */
        uint8_t faceMask;
    };

    /**
     * Соседние блоки.
     */
    std::array<Chunk*, 6> neighbours = { nullptr };

    /**
     * Трехмерная граница блока.
     */
    std::vector<BorderInfo> border;

    /**
     * Данные в блоке.
     */
    std::array<std::array<std::array<uint8_t, length>, height>, width> data = { 0 };
};