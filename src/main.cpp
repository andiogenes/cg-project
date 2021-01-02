/*
 * Anton Zavyalov, Altai STU, 2020.
 */

#include "Application.hpp"
#include "renderers/ChunkRenderer.hpp"

int main()
{
    const auto screenWidth = 1280;
    const auto screenHeight = 768;

    auto app = Application(screenWidth, screenHeight, "GPU-baked voxel render");
    auto renderer = ChunkRenderer(32, 32, 8);
    app.run(renderer);
}
