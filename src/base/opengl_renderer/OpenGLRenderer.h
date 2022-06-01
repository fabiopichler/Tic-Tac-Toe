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

#include "GL.h"
#include "GLProgram.h"
#include "GLBuffer.h"
#include "GLTexture.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Rect Rect;
typedef struct IRect IRect;
typedef struct Color Color;

typedef struct OpenGLRenderer OpenGLRenderer;

OpenGLRenderer *OpenGLRenderer_New();
void OpenGLRenderer_Delete(OpenGLRenderer * const self);

void OpenGLRenderer_InitGL(OpenGLRenderer * const self);

Texture2D *OpenGLRenderer_CreateTexture(OpenGLRenderer * const self, const Image *image, TextureFilter filter);
void OpenGLRenderer_DestroyTexture(OpenGLRenderer * const self, Texture2D *texture);
void OpenGLRenderer_Clear(OpenGLRenderer * const self);

void OpenGLRenderer_Draw(OpenGLRenderer * const self, const Texture2D *texture, const IRect *srcrect, const Rect *dstrect, const float angle);
void OpenGLRenderer_FillRect(OpenGLRenderer * const self, const Rect *rect, const Color *color);

void OpenGLRenderer_SetViewportSize(OpenGLRenderer * const self, int w, int h);
void OpenGLRenderer_SetLogicalSize(OpenGLRenderer * const self, int w, int h);

#ifdef __cplusplus
}
#endif
