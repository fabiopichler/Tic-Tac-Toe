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

#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/mat4.h>
#include <cglm/affine.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Vertex
{
    vec2 position;
    vec2 UV;
} Vertex;

typedef struct GLProgramLocation GLProgramLocation;

typedef struct GLBuffer GLBuffer;

GLBuffer *GLBuffer_New();
void GLBuffer_Delete(GLBuffer * const self);

void GLBuffer_Init(GLBuffer * const self);
void GLBuffer_EnablePositionVBO(GLBuffer * const self, const GLProgramLocation *program);
void GLBuffer_DisablePositionVBO(GLBuffer * const self, const GLProgramLocation *program);

void GLBuffer_EnableColorVBO(GLBuffer * const self, const GLProgramLocation *program, const vec4 colors[4]);
void GLBuffer_DisableColorVBO(GLBuffer * const self, const GLProgramLocation *program);

void GLBuffer_DrawElements(GLBuffer * const self);

#ifdef __cplusplus
}
#endif
