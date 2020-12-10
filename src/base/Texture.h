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

typedef struct Texture Texture;

Texture *Texture_New(SDL_Renderer *renderer);
void Texture_Delete(Texture *const this);

bool Texture_LoadImageFromFile(Texture *const this, const char *fileName);

bool Texture_MakeText(Texture *const this);
void Texture_SetupText(Texture *const this, const char *text, int ptsize, SDL_Color color);
void Texture_SetText(Texture *const this, const char *text);
void Texture_SetTextSize(Texture *const this, int ptsize);
void Texture_SetTextColor(Texture *const this, SDL_Color color);

void Texture_Draw(Texture *const this, const SDL_Rect *srcrect, const SDL_Rect *dstrect);
void Texture_SetPos(Texture *const this, int x, int y);
void Texture_SetRect(Texture *const this, SDL_Rect rect);
int Texture_GetWidth(Texture *const this);
int Texture_GetHeight(Texture *const this);
