#pragma once

#include "../atlas/TextureAtlas.hpp"

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render2D() = 0;
    virtual void render3D() = 0;

protected:
    TextureAtlas atlas = TextureAtlas("resources/atlas.png", {
            {"grass", {0, 0, 64, 64}}
    });
};