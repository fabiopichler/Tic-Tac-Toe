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

#include "Texture.h"

#include <stdbool.h>

#include <SDL2/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Texture Texture;

typedef struct Button Button;

typedef void (*Button_OnPressEvent)(Button * const button, void *user);

Button *Button_New(SDL_Renderer *renderer);
void Button_Delete(Button * const self);
void Button_SetBackgroundColorRGB(Button * const self, uint8_t r, uint8_t g, uint8_t b);
void Button_SetBackgroundColorRGBA(Button * const self, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void Button_SetBackgroundHoverColorRGB(Button * const self, uint8_t r, uint8_t g, uint8_t b);
void Button_SetBackgroundHoverColorRGBA(Button * const self, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void Button_SetBackgroundPressedColorRGB(Button * const self, uint8_t r, uint8_t g, uint8_t b);
void Button_SetBackgroundPressedColorRGBA(Button * const self, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void Button_SetTextColorRGB(Button * const self, uint8_t r, uint8_t g, uint8_t b);
void Button_SetTextColorRGBA(Button * const self, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
bool Button_SetText(Button * const self, const char *text, int ptsize);
void Button_SetIcon(Button * const self, Texture *texture);
void Button_SetOnPressEvent(Button * const self, Button_OnPressEvent callback, void *userdata);
void *Button_GetEventUserData(Button * const self);
void Button_ProcessEvent(Button * const self, const SDL_Event *event);
void Button_Draw(Button * const self);

Box *Button_Box(Button * const self);
Texture *Button_Icon(Button * const self);

#ifdef __cplusplus
}
#endif
