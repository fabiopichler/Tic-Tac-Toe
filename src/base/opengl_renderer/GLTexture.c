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

#include "GLTexture.h"

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

static void GetFormat(const Image *image, Texture2D *texture, int *mode, uint32_t *format);
static void SetTextureFilter(TextureFilter filter);

struct GLTexture
{

};

GLTexture *GLTexture_New()
{
    GLTexture * const self = malloc(sizeof (GLTexture));

    return self;
}

void GLTexture_Delete(GLTexture * const self)
{
    if (!self)
        return;

    free(self);
}

void GLTexture_Init(GLTexture * const self)
{
}

Texture2D *GLTexture_CreateTexture(GLTexture * const self, const Image *image, TextureFilter filter)
{
    int mode;
    uint32_t format;
    const int rowLength = image->pitch / image->bytesPerPixel;
    Texture2D *texture = malloc(sizeof (Texture2D));

    texture->width = image->width;
    texture->height = image->height;
#ifdef RENDERER_GL_ES
    texture->format = RGBA;
#endif

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    GetFormat(image, texture, &mode, &format);

#ifdef RENDERER_GL_ES
    if (!IsOpenGL_ES_3())
    {
        if (rowLength != image->width)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, mode, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, NULL);

            for (int y = 0; y < image->height; y++)
            {
                unsigned char *row = image->pixels + (y * rowLength) * 4;
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, y, image->width, 1, format, GL_UNSIGNED_BYTE, row);
            }
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, mode, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->pixels);
        }
    }
    else
#endif
    {
        if (rowLength != image->width)
            glPixelStorei(GL_UNPACK_ROW_LENGTH, rowLength);

        glTexImage2D(GL_TEXTURE_2D, 0, mode, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->pixels);

        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    }

    if (glGetError() != GL_NO_ERROR)
    {
        puts("Texture error");
        free(texture);
        exit(EXIT_FAILURE);
    }

    SetTextureFilter(filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void GLTexture_DestroyTexture(GLTexture * const self, Texture2D *texture)
{
    if (texture)
    {
        glDeleteTextures(1, &texture->id);
        free(texture);
    }
}

void GetFormat(const Image *image, Texture2D *texture, int *mode, uint32_t *format)
{
    if (image->bytesPerPixel == 4)
    {
        *mode = GL_RGBA;
        *format = GL_RGBA;

        if (image->rmask != 0x000000ff)
        {
#ifndef RENDERER_GL_ES
            *format = GL_BGRA;
#else
            texture->format = BGRA;
#endif
        }
    }
    else
    {
        *mode = GL_RGB;
        *format = GL_RGB;

        if (image->rmask != 0x000000ff)
        {
#ifndef RENDERER_GL_ES
            *format = GL_BGR;
#else
            texture->format = BGRA;
#endif
        }
    }
}

void SetTextureFilter(TextureFilter filter)
{
    if (filter == Linear)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else if (filter == Mipmap
#ifndef RENDERER_GL_ES
             && IsOpenGL_3()
#endif
             )
    {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}
