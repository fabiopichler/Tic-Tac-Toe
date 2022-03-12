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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Button Button;

typedef void (*ButtonOnPressEvent)(Button *const button, void *user);

Button *Button_New(SDL_Renderer *renderer);
void Button_Delete(Button *self);
void Button_SetBackgroundColor(Button *self, const SDL_Color *color);
void Button_SetBackgroundHoverColor(Button *self, const SDL_Color *color);
void Button_SetBackgroundPressedColor(Button *self, const SDL_Color *color);
void Button_SetTextColor(Button *self, const SDL_Color *color);
bool Button_SetText(Button *self, const char *text, int ptsize);
void Button_SetImage(Button *self, Texture *texture);
void Button_SetRect(Button *self, const SDL_Rect *rect);
void Button_SetOnPressEvent(Button *self, ButtonOnPressEvent callback, void *user);
void *Button_GetEventUserData(Button *self);
void Button_ProcessEvent(Button *self, const SDL_Event *event);
void Button_Draw(Button *self);
void Button_DrawEx(Button *self, const SDL_Rect *srcrect, const SDL_Rect *dstrect, const double angle);

#ifdef __cplusplus
}
#endif
