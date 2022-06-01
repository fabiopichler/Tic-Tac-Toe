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

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum GLProgramLocation_Type
{
    Type_Color = 0,
    Type_Texture = 1,
    Type_TextureBGRA = 2,
    _Type_size = 3
} GLProgramLocation_Type;

typedef struct GLProgramLocation
{
    GLuint program;
    GLint aPosition;
    GLint aUV;
    GLint aColor;
    GLint uProjection;
    GLint uSampler;
    GLint uSourcePosition;
    GLint uTransform;
} GLProgramLocation;

typedef struct GLProgram GLProgram;

GLProgram *GLProgram_New();
void GLProgram_Delete(GLProgram * const self);

const GLProgramLocation *GLProgram_InitProgram(GLProgram * const self, GLProgramLocation_Type type);
const GLProgramLocation *GLProgram_GetProgram(GLProgram * const self, GLProgramLocation_Type type);

#ifdef __cplusplus
}
#endif
