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

#include <stdbool.h>

#include <SDL2/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Box Box;

typedef struct Texture Texture;

Texture *Texture_New(SDL_Renderer *renderer);
void Texture_Delete(Texture * const self);

bool Texture_LoadImageFromFile(Texture * const self, const char *fileName);

bool Texture_MakeText(Texture * const self);
void Texture_SetText(Texture * const self, const char *text);
void Texture_SetTextSize(Texture * const self, int ptsize);
void Texture_SetTextColor(Texture * const self, const SDL_Color *color);
void Texture_SetTextColorRGB(Texture * const self, uint8_t r, uint8_t g, uint8_t b);
void Texture_SetTextColorRGBA(Texture * const self, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

void Texture_SetSourceRect(Texture * const self, SDL_Rect srcrect);
void Texture_SetAngle(Texture * const self, double angle);

void Texture_Draw(Texture * const self);

int Texture_GetWidth(Texture * const self);
int Texture_GetHeight(Texture * const self);
Box *Texture_Box(Texture * const self);

#ifdef __cplusplus
}
#endif
