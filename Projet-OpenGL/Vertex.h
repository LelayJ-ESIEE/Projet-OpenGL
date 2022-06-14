#pragma once

#include <cstdint>

struct vec2 {
    float x, y;
};

struct vec3 {
    float x, y, z;
};

struct Color {
    uint8_t r, g, b, a;
};

struct Vertex
{
    vec2 position;
    vec2 uv;
    Color color;
};
