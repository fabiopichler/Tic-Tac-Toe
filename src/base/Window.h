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

#include "rect.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct SDL_Window SDL_Window;

typedef struct IRect IRect;
typedef struct Window Window;

Window *Window_New(int width, int height, const char *title);
void Window_Delete(Window * const self);
void Window_SetWindowIcon(Window * const self, const char *filename);
void Window_SetWindowTitle(Window * const self, const char *title);
SDL_Window *Window_GetSDLWindow(Window * const self);
IVec2 Window_GetSize(Window * const self);
void Window_SwapWindow(Window * const self);

#ifdef __cplusplus
}
#endif
