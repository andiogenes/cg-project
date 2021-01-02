/*
 * Anton Zavyalov, Altai STU, 2020.
 */

#pragma once

#include "raylib.h"
#include "renderers/Renderer.hpp"

/**
 * Каркас приложения с задаваемым отрисовщиком.
 */
class Application {
public:
    /**
     * @param width Ширина окна
     * @param height Длина окна
     * @param title Заголовок окна
     * @param fullscreen Полноэкранный режим
     */
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

    /**
     * Запускает приложение с установленной стратегией отрисовки.
     * @param renderer Отрисовщик, согласно которому отображается графика
     */
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