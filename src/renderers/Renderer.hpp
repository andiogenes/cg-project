/*
 * Anton Zavyalov, Altai STU, 2020.
 */

#pragma once

#include "../atlas/TextureAtlas.hpp"

/**
 * Отрисовщик - стратегия, по которой происходит визуализация трехмерной графики.
 */
class Renderer {
public:
    virtual ~Renderer() = default;

    /**
     * Отрисовка двухмерной графики в экранной плоскости.
     */
    virtual void render2D() = 0;

    /**
     * Отрисовка трехмерной графики.
     * @param camera Информация о наблюдателе
     */
    virtual void render3D(const Camera3D &camera) = 0;

protected:
    /**
     * Стандартный атлас с текстурами.
     */
    TextureAtlas atlas = TextureAtlas("resources/atlas.png", {
            {"grass", {0, 0, 64, 64}},
            {"dirt", {64, 0, 64, 64}},
            {"sand", {128, 0, 64, 64}}
    });
};