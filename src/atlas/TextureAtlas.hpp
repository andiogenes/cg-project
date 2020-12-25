#pragma once

#include <map>
#include <string>
#include "raylib.h"

class TextureAtlas {
public:
    TextureAtlas(const char *fileName, const std::map<std::string, Rectangle> &textureInfo) {
        Image image = LoadImage(fileName);

        for (auto const &t : textureInfo) {
            Image rect = ImageFromImage(image, t.second);
            textures[t.first] = LoadTextureFromImage(rect);
            UnloadImage(rect);
        }

        UnloadImage(image);
    }

    ~TextureAtlas() {
        for (auto const &t : textures) {
            UnloadTexture(t.second);
        }
    }

    const Texture2D& operator[](const std::string &key) {
        return textures[key];
    }

private:
    std::map<std::string, Texture2D> textures;
};