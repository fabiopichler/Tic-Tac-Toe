//-------------------------------------------------------------------------------
// Copyright (c) 2020-2022 Fábio Pichler
/*-------------------------------------------------------------------------------

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-------------------------------------------------------------------------------*/

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Color Color;
typedef struct OpenGLRenderer OpenGLRenderer;
typedef struct Box Box;

typedef struct Rectangle Rectangle;

Rectangle *Rectangle_New(OpenGLRenderer *renderer, float width, float height);
void Rectangle_Delete(Rectangle * const self);
void Rectangle_Draw(Rectangle * const self);

void Rectangle_SetColor(Rectangle * const self, Color color);
void Rectangle_SetColorRGB(Rectangle * const self, uint8_t r, uint8_t g, uint8_t b);
void Rectangle_SetColorRGBA(Rectangle * const self, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
Color Rectangle_Color(Rectangle * const self);

Box *Rectangle_Box(Rectangle * const self);

#ifdef __cplusplus
}
#endif
