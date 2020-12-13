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

#include "Texture.h"

#include <stdbool.h>

#include <SDL2/SDL.h>

typedef struct Button Button;

typedef void (*ButtonOnPressEvent)(Button *const button, void *user);

Button *Button_New(SDL_Renderer *renderer);
void Button_Delete(Button *const this);
void Button_SetBackgroundColor(Button *const this, SDL_Color color);
void Button_SetBackgroundHoverColor(Button *const this, SDL_Color color);
void Button_SetBackgroundPressedColor(Button *const this, SDL_Color color);
void Button_SetTextColor(Button *const this, SDL_Color color);
bool Button_SetText(Button *const this, const char *text, int ptsize);
void Button_SetImage(Button *const this, Texture *texture);
void Button_SetRect(Button *const this, SDL_Rect rect);
void Button_SetOnPressEvent(Button *const this, ButtonOnPressEvent callback, void *user);
void Button_ProcessEvent(Button *const this, const SDL_Event *event);
void Button_Draw(Button *const this);
void Button_DrawEx(Button *const this, const SDL_Rect *srcrect, const SDL_Rect *dstrect, const double angle);
