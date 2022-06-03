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
#include "../TextureFilter.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef RENDERER_GL_ES
typedef enum Texture2DFormat
{
    RGBA,
    BGRA,
} Texture2DFormat;
#endif

typedef struct Texture2D
{
    GLuint id;
    float width;
    float height;
#ifdef RENDERER_GL_ES
    Texture2DFormat format;
#endif
} Texture2D;

typedef struct Image
{
    int width;
    int height;
    int bytesPerPixel;
    int pitch;
    uint32_t rmask;
    unsigned char *pixels;
} Image;

typedef struct GLTexture GLTexture;

GLTexture *GLTexture_New();
void GLTexture_Delete(GLTexture * const self);

void GLTexture_Init(GLTexture * const self);

Texture2D *GLTexture_CreateTexture(GLTexture * const self, const Image *image, TextureFilter filter);
void GLTexture_DestroyTexture(GLTexture * const self, Texture2D *texture);

#ifdef __cplusplus
}
#endif
