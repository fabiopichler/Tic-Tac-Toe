//-------------------------------------------------------------------------------
// Copyright (c) 2020 Fábio Pichler
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

#include "stdbool.h"

#include <SDL2/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Texture Texture;

Texture *Texture_New(SDL_Renderer *renderer);
void Texture_Delete(Texture *self);

bool Texture_LoadImageFromFile(Texture *self, const char *fileName);

bool Texture_MakeText(Texture *self);
void Texture_SetupText(Texture *self, const char *text, int ptsize, const SDL_Color *color);
void Texture_SetText(Texture *self, const char *text);
void Texture_SetTextSize(Texture *self, int ptsize);
void Texture_SetTextColor(Texture *self, const SDL_Color *color);

void Texture_Draw(Texture *self, const SDL_Rect *srcrect, const SDL_Rect *dstrect);
void Texture_DrawEx(Texture *self, const SDL_Rect *srcrect, const SDL_Rect *dstrect, const double angle);
void Texture_SetPos(Texture *self, int x, int y);
void Texture_SetRect(Texture *self, const SDL_Rect *rect);
int Texture_GetWidth(Texture *self);
int Texture_GetHeight(Texture *self);

#ifdef __cplusplus
}
#endif
