#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Vec2
{
    float x, y;
} Vec2;

typedef struct Rect
{
    float x, y, w, h;
} Rect;

typedef struct IRect
{
    int x, y, w, h;
} IRect;

typedef struct Color
{
    int r, g, b, a;
} Color;

#ifdef __cplusplus
}
#endif
