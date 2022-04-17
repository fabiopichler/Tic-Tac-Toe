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

#include "Rectangle.h"
#include "Box.h"

struct Rectangle
{
    SDL_Renderer *renderer;
    Box *box;
    SDL_Color color;
};

Rectangle *Rectangle_New(SDL_Renderer *renderer, float width, float height)
{
    Rectangle * const self = malloc(sizeof (Rectangle));

    self->renderer = renderer;
    self->box = Box_New(0.f, 0.f, width, height);
    self->color = (SDL_Color) {0, 0, 0, 0};

    return self;
}

void Rectangle_Delete(Rectangle * const self)
{
    if (!self)
        return;

    Box_Delete(self->box);
    free(self);
}

void Rectangle_Draw(Rectangle * const self)
{
    SDL_SetRenderDrawColor(self->renderer, self->color.r, self->color.g, self->color.b, self->color.a);
    SDL_RenderFillRectF(self->renderer, Box_Rect(self->box));
}

void Rectangle_SetColor(Rectangle * const self, SDL_Color color)
{
    self->color = color;
}

void Rectangle_SetColorRGB(Rectangle * const self, uint8_t r, uint8_t g, uint8_t b)
{
    self->color.r = r;
    self->color.g = g;
    self->color.b = b;
    self->color.a = 255;
}

void Rectangle_SetColorRGBA(Rectangle * const self, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    self->color.r = r;
    self->color.g = g;
    self->color.b = b;
    self->color.a = a;
}

SDL_Color Rectangle_Color(Rectangle * const self)
{
    return self->color;
}

Box *Rectangle_Box(Rectangle * const self)
{
    return self->box;
}
