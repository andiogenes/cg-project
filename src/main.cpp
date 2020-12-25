#include "Application.hpp"
#include "renderers/NaiveRenderer.hpp"

int main()
{
    const auto screenWidth = 1280;
    const auto screenHeight = 768;

    auto app = Application(screenWidth, screenHeight, "minesruft");
    auto renderer = NaiveRenderer(32, 32, 8);
    app.run(renderer);
}
