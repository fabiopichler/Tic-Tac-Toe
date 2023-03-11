#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Vec2
{
    union { float x, w; };
    union { float y, h; };
} Vec2;

typedef struct IVec2
{
    union { int x, w; };
    union { int y, h; };
} IVec2;

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
