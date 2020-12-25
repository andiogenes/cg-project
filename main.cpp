#include "raylib.h"

int main()
{
    const auto screenWidth = 1280;
    const auto screenHeight = 768;

    InitWindow(screenWidth, screenHeight, "minesruft");

    Camera camera = {
            { -1.0f, 0.0f, -1.0f },
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            60.0f,
            CAMERA_PERSPECTIVE
    };

    Texture2D texture;
    {
        Image image = LoadImage("resources/atlas.png");
        Image grass = ImageFromImage(image, {0, 0, 64, 64});

        texture = LoadTextureFromImage(grass);

        UnloadImage(grass);
        UnloadImage(image);
    }

    Model cube = LoadModelFromMesh(GenMeshCube(1.f, 1.f, 1.f));
    cube.materials[0].maps[MAP_DIFFUSE].texture = texture;

    SetCameraMode(camera, CAMERA_FIRST_PERSON);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateCamera(&camera);

        BeginDrawing();

            ClearBackground(SKYBLUE);

            BeginMode3D(camera);
                for (int x = 0; x < 16; x++) {
                    for (int y = 0; y < 16; y++) {
                        for (int z = 0; z < 16; z++) {
                            DrawModel(cube, {(float)x, (float)y, (float)z}, 1.f, WHITE);
                        }
                    }
                }
            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadModel(cube);
    UnloadTexture(texture);

    CloseWindow();
}
