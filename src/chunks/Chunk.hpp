#pragma once

#include "raylib.h"
#include "rlgl.h"
#include <array>
#include <vector>
#include <cstring>

#define NEIGHBOUR_UP 0
#define NEIGHBOUR_DOWN 1
#define NEIGHBOUR_LEFT 2
#define NEIGHBOUR_RIGHT 3
#define NEIGHBOUR_FORWARD 4
#define NEIGHBOUR_BACKWARD 5

#define DEFAULT_MESH_VERTEX_BUFFERS 7

template<size_t width, size_t height, size_t length>
struct Chunk {
    ~Chunk() {
        for (const auto &model : models) { UnloadModel(model); }
    }

    void calculateBorder(int worldWidth, int worldHeight, int worldLength) {
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

    void genMeshes() {
        for (int i = 0; i < 3; i++) {
            meshes[i] = { 0 };
            meshes[i].vboId = (unsigned int *)RL_CALLOC(DEFAULT_MESH_VERTEX_BUFFERS, sizeof(unsigned int));
        }

        for (const auto &it : border) {
            const auto &pos = it.pos;

            auto &_vertices = vertices[it.tpe];
            auto &_normals = normals[it.tpe];
            auto &_texcoords = texcoords[it.tpe];

            _texcoords.push_back(0.f); _texcoords.push_back(0.f);
            _texcoords.push_back(1.f); _texcoords.push_back(0.f);
            _texcoords.push_back(1.f); _texcoords.push_back(1.f);
            _texcoords.push_back(0.f); _texcoords.push_back(1.f);

            if ((it.faceMask >> 0) & 0x1) {
                _vertices.push_back(pos.x + 0.5f); _vertices.push_back(pos.y - 0.5f); _vertices.push_back(pos.z - 0.5f);
                _vertices.push_back(pos.x + 0.5f); _vertices.push_back(pos.y - 0.5f); _vertices.push_back(pos.z + 0.5f);
                _vertices.push_back(pos.x + 0.5f); _vertices.push_back(pos.y + 0.5f); _vertices.push_back(pos.z + 0.5f);
                _vertices.push_back(pos.x + 0.5f); _vertices.push_back(pos.y + 0.5f); _vertices.push_back(pos.z - 0.5f);

                for (int i = 0; i < 4; i++) { _normals.push_back(1.f); _normals.push_back(0.f); _normals.push_back(0.f); }
            }
            if ((it.faceMask >> 1) & 0x1) {
                _vertices.push_back(pos.x - 0.5f); _vertices.push_back(pos.y + 0.5f); _vertices.push_back(pos.z - 0.5f);
                _vertices.push_back(pos.x - 0.5f); _vertices.push_back(pos.y + 0.5f); _vertices.push_back(pos.z + 0.5f);
                _vertices.push_back(pos.x - 0.5f); _vertices.push_back(pos.y - 0.5f); _vertices.push_back(pos.z + 0.5f);
                _vertices.push_back(pos.x - 0.5f); _vertices.push_back(pos.y - 0.5f); _vertices.push_back(pos.z - 0.5f);

                for (int i = 0; i < 4; i++) { _normals.push_back(-1.f); _normals.push_back(0.f); _normals.push_back(0.f); }
            }
            if ((it.faceMask >> 2) & 0x1) {
                _vertices.push_back(pos.x - 0.5f); _vertices.push_back(pos.y - 0.5f); _vertices.push_back(pos.z + 0.5f);
                _vertices.push_back(pos.x + 0.5f); _vertices.push_back(pos.y - 0.5f); _vertices.push_back(pos.z + 0.5f);
                _vertices.push_back(pos.x + 0.5f); _vertices.push_back(pos.y + 0.5f); _vertices.push_back(pos.z + 0.5f);
                _vertices.push_back(pos.x - 0.5f); _vertices.push_back(pos.y + 0.5f); _vertices.push_back(pos.z + 0.5f);

                for (int i = 0; i < 4; i++) { _normals.push_back(0.f); _normals.push_back(0.f); _normals.push_back(1.f); }
            }
            if ((it.faceMask >> 3) & 0x1) {
                _vertices.push_back(pos.x - 0.5f); _vertices.push_back(pos.y + 0.5f); _vertices.push_back(pos.z - 0.5f);
                _vertices.push_back(pos.x + 0.5f); _vertices.push_back(pos.y + 0.5f); _vertices.push_back(pos.z - 0.5f);
                _vertices.push_back(pos.x + 0.5f); _vertices.push_back(pos.y - 0.5f); _vertices.push_back(pos.z - 0.5f);
                _vertices.push_back(pos.x - 0.5f); _vertices.push_back(pos.y - 0.5f); _vertices.push_back(pos.z - 0.5f);

                for (int i = 0; i < 4; i++) { _normals.push_back(0.f); _normals.push_back(0.f); _normals.push_back(-1.f); }
            }
            if ((it.faceMask >> 4) & 0x1) {
                _vertices.push_back(pos.x - 0.5f); _vertices.push_back(pos.y + 0.5f); _vertices.push_back(pos.z - 0.5f);
                _vertices.push_back(pos.x - 0.5f); _vertices.push_back(pos.y + 0.5f); _vertices.push_back(pos.z + 0.5f);
                _vertices.push_back(pos.x + 0.5f); _vertices.push_back(pos.y + 0.5f); _vertices.push_back(pos.z + 0.5f);
                _vertices.push_back(pos.x + 0.5f); _vertices.push_back(pos.y + 0.5f); _vertices.push_back(pos.z - 0.5f);

                for (int i = 0; i < 4; i++) { _normals.push_back(0.f); _normals.push_back(1.f); _normals.push_back(0.f); }
            }
            if ((it.faceMask >> 5) & 0x1) {
                _vertices.push_back(pos.x + 0.5f); _vertices.push_back(pos.y - 0.5f); _vertices.push_back(pos.z - 0.5f);
                _vertices.push_back(pos.x + 0.5f); _vertices.push_back(pos.y - 0.5f); _vertices.push_back(pos.z + 0.5f);
                _vertices.push_back(pos.x - 0.5f); _vertices.push_back(pos.y - 0.5f); _vertices.push_back(pos.z + 0.5f);
                _vertices.push_back(pos.x - 0.5f); _vertices.push_back(pos.y - 0.5f); _vertices.push_back(pos.z - 0.5f);

                for (int i = 0; i < 4; i++) { _normals.push_back(0.f); _normals.push_back(-1.f); _normals.push_back(0.f); }
            }
        }

        for (int i = 0; i < 3; i++) {
            auto &mesh = meshes[i];

            auto &_vertices = vertices[i];
            auto &_normals = normals[i];
            auto &_texcoords = texcoords[i];

            mesh.vertices = (float *)RL_MALLOC(_vertices.size() * 3 * sizeof(float));
            memcpy(mesh.vertices, _vertices.data(), _vertices.size() * 3 * sizeof(float));

            mesh.texcoords = (float *)RL_MALLOC(_texcoords.size() * 2 * sizeof(float));
            memcpy(mesh.texcoords, _texcoords.data(), _texcoords.size() * 2 * sizeof(float));

            mesh.normals = (float *)RL_MALLOC(_normals.size() * 3 * sizeof(float));
            memcpy(mesh.normals, _normals.data(), _normals.size() * 3 * sizeof(float));

            int triangleCount = _vertices.size() / 2;
            mesh.indices = (unsigned short *)RL_MALLOC(triangleCount * 3 * sizeof(unsigned short));

            int k = 0;

            for (int j = 0; j < triangleCount * 3; j += 6)
            {
                mesh.indices[j] = 4 * k;
                mesh.indices[j + 1] = 4 * k + 1;
                mesh.indices[j + 2] = 4 * k + 2;
                mesh.indices[j + 3] = 4 * k;
                mesh.indices[j + 4] = 4 * k + 2;
                mesh.indices[j + 5] = 4 * k + 3;

                k++;
            }

            mesh.vertexCount = _vertices.size();
            mesh.triangleCount = triangleCount;

            rlLoadMesh(&mesh, false);

            models[i] = LoadModelFromMesh(mesh);
        }
    }

    Vector3 offset;

    struct BorderInfo {
        Vector3 pos;
        uint8_t tpe;
        uint8_t faceMask;
    };

    std::array<Chunk*, 6> neighbours = { nullptr };
    std::vector<BorderInfo> border;
    std::array<std::array<std::array<uint8_t, length>, height>, width> data = { 0 };

    std::array<Mesh, 3> meshes;
    std::array<std::vector<float>, 3> vertices;
    std::array<std::vector<float>, 3> normals;
    std::array<std::vector<float>, 3> texcoords;

    std::array<Model, 3> models;
};