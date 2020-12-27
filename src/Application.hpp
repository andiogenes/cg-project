#pragma once

#include "raylib.h"
#include "renderers/Renderer.hpp"

class Application {
public:
    Application(int width, int height, const char* title, bool fullscreen = false) {
        ConfigFlag configFlags = FLAG_MSAA_4X_HINT;
        if (fullscreen) {
            configFlags = (ConfigFlag)(configFlags | FLAG_FULLSCREEN_MODE);
        }
        SetConfigFlags(configFlags);

        InitWindow(width, height, title);
        SetCameraMode(camera, CAMERA_FIRST_PERSON);
        SetTargetFPS(60);
    }

    ~Application() {
        CloseWindow();
    }

    void run(Renderer &renderer) {
        while (!WindowShouldClose()) {
            UpdateCamera(&camera);

            BeginDrawing();

                ClearBackground(SKYBLUE);

                BeginMode3D(camera);
                    renderer.render3D(camera);
                EndMode3D();

                renderer.render2D();
                DrawFPS(10, 10);

            EndDrawing();
        }
    }

private:
    Camera camera = {
            {-1.0f, 2.0f, -1.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            60.0f,
            CAMERA_PERSPECTIVE
    };
};