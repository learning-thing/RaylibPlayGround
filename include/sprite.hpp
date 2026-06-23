//
// Created by benji on 23.06.26.
//

#ifndef RAYLIBPLAYGROUND_SPRITE_HPP
#define RAYLIBPLAYGROUND_SPRITE_HPP
#include <vector>

#include <cstdint>

#include "raylib.h"

struct Sprite_t {
    uint16_t width;
    uint16_t frameCount;
    Texture texture;
};

inline std::vector<Sprite_t> sprites;

inline unsigned int loadSprite(const char *path, unsigned int frameCount) {
    const auto t = LoadTexture(path);

    sprites.emplace_back(t.width/frameCount, frameCount, t);
    return sprites.size()-1;
}

inline void drawSprite(const int spriteID, int frame, const float x, const float y, float Dwidth, float Dheight) {
    const auto&[width, frameCount, texture] = sprites[spriteID];
    if (frame < 0) frame = 0;
    if (frame > frameCount-1) frame = frameCount-1;
    DrawTexturePro(texture,
        {static_cast<float>(frame*width), 0, static_cast<float>(width), static_cast<float>(texture.height)},
        {x, y, static_cast<float>(Dwidth), static_cast<float>(Dheight)},
        {0, 0},
        0,
        WHITE);
}

#endif //RAYLIBPLAYGROUND_SPRITE_HPP
