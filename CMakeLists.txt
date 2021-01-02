cmake_minimum_required(VERSION 3.17)
project(minesruft)

find_package(raylib 2.5.0 REQUIRED PATHS raylib/build)

set(CMAKE_CXX_STANDARD 14)

add_executable(minesruft
        src/main.cpp
        src/atlas/TextureAtlas.hpp
        src/Application.hpp
        src/renderers/Renderer.hpp
        src/renderers/NaiveRenderer.hpp
        src/renderers/ChunkRenderer.hpp
        src/chunks/Chunk.hpp
        )

target_link_libraries(minesruft raylib)