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
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int mode;
    uint32_t format;

#ifdef RENDERER_GLES
    Texture2DFormat texformat = RGBA;
#endif

    if (image->bytesPerPixel == 4)
    {
        mode = GL_RGBA;

        if (image->rmask == 0x000000ff)
        {
            format = GL_RGBA;
        }
        else
        {
#ifndef RENDERER_GLES
            format = GL_BGRA;
#else
            format = GL_RGBA;
            texformat = BGRA;
#endif
        }
    }
    else
    {
        mode = GL_RGB;

        if (image->rmask == 0x000000ff)
        {
            format = GL_RGB;
        }
        else
        {
#ifndef RENDERER_GLES
            format = GL_BGR;
#else
            format = GL_RGB;
            texformat = BGRA;
#endif
        }
    }

    if (image->pitch / image->bytesPerPixel != image->width)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, image->pitch / image->bytesPerPixel);

    glTexImage2D(GL_TEXTURE_2D, 0, mode, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->pixels);

    if (glGetError() != GL_NO_ERROR)
    {
        puts("Texture error");
        exit(EXIT_FAILURE);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    SetTextureFilter(filter);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    Texture2D *texture2D = malloc(sizeof (Texture2D));
    texture2D->id = texture;
    texture2D->width = image->width;
    texture2D->height = image->height;
#ifdef RENDERER_GLES
    texture2D->format = texformat;
#endif

    return texture2D;
}

void GLTexture_DestroyTexture(GLTexture * const self, Texture2D *texture)
{
    if (texture)
    {
        glDeleteTextures(1, &texture->id);
        free(texture);
    }
}

void SetTextureFilter(TextureFilter filter)
{
    if (filter == Linear)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
#ifndef RENDERER_GL2
    else if (filter == Mipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
#endif
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}
