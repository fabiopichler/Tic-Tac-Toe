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

#include <SDL2/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Box Box;
typedef void (*Box_OnUpdateEvent)(Box * const box, void *userdata);

Box *Box_New(float x, float y, float width, float height);
void Box_Delete(Box * const self);

void Box_SetOnPressEvent(Box * const self, Box_OnUpdateEvent callback, void *userdata);
void *Box_GetEventUserData(Box * const self);

void Box_SetSize(Box * const self, float w, float h);
void Box_SetPosition(Box * const self, float x, float y);

void Box_SetX(Box * const self, float x);
void Box_SetY(Box * const self, float y);
void Box_SetWidth(Box * const self, float w);
void Box_SetHeight(Box * const self, float h);

void Box_Move(Box * const self, float velX, float velY);

float Box_X(Box * const self);
float Box_Y(Box * const self);
float Box_Width(Box * const self);
float Box_Height(Box * const self);

const SDL_FRect *Box_Rect(Box * const self);

#ifdef __cplusplus
}
#endif
