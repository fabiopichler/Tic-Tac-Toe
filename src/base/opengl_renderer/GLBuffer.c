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

#include "GLBuffer.h"
#include "GLProgram.h"

#include <malloc.h>

struct GLBuffer
{
    GLuint vao;
    GLuint positionVBO, colorVBO, elementBuffer;
    int indicesSize;
};

GLBuffer *GLBuffer_New()
{
    GLBuffer * const self = malloc(sizeof (GLBuffer));

    self->vao = 0;
    self->positionVBO = 0;
    self->colorVBO = 0;
    self->elementBuffer = 0;
    self->indicesSize = 0;

    return self;
}

void GLBuffer_Delete(GLBuffer * const self)
{
    if (!self)
        return;

    glDeleteBuffers(1, &self->positionVBO);
    glDeleteBuffers(1, &self->elementBuffer);
    glDeleteBuffers(1, &self->colorVBO);

#ifndef RENDERER_GL_ES
    if (IsOpenGL_3())
        glDeleteVertexArrays(1, &self->vao);
#endif

    free(self);
}

void GLBuffer_Init(GLBuffer * const self)
{
#ifndef RENDERER_GL_ES
    if (IsOpenGL_3())
    {
        glGenVertexArrays(1, &self->vao);
        glBindVertexArray(self->vao);
    }
#endif

    const ivec3 indices[2] = {
        {0, 1, 2},
        {0, 2, 3},
    };

    self->indicesSize = sizeof (indices) / sizeof (int);

    const Vertex vertices[4] = {
        {{0.0f, 1.0f}, {0.0f, 1.0f}},
        {{1.0f, 1.0f}, {1.0f, 1.0f}},
        {{1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.0f, 0.0f}, {0.0f, 0.0f}},
    };

    glGenBuffers(1, &self->elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &self->positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, self->positionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &self->colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, self->colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof (vec4[4]), NULL, GL_DYNAMIC_DRAW);
}

void GLBuffer_EnablePositionVBO(GLBuffer * const self, const GLProgramLocation *program)
{
    glBindBuffer(GL_ARRAY_BUFFER, self->positionVBO);

    glEnableVertexAttribArray(program->aPosition);
    glVertexAttribPointer(program->aPosition, 2, GL_FLOAT, GL_FALSE, sizeof (Vertex), NULL);

    if (program->aUV != -1)
    {
        glEnableVertexAttribArray(program->aUV);
        glVertexAttribPointer(program->aUV, 2, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void *) offsetof(Vertex, UV));
    }
}

void GLBuffer_DisablePositionVBO(GLBuffer * const self, const GLProgramLocation *program)
{
    glDisableVertexAttribArray(program->aPosition);

    if (program->aUV != -1)
        glDisableVertexAttribArray(program->aUV);
}

void GLBuffer_EnableColorVBO(GLBuffer * const self, const GLProgramLocation *program, const vec4 colors[4])
{
    glEnableVertexAttribArray(program->aColor);
    glBindBuffer(GL_ARRAY_BUFFER, self->colorVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof (vec4[4]), colors);
    glVertexAttribPointer(program->aColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void GLBuffer_DisableColorVBO(GLBuffer * const self, const GLProgramLocation *program)
{
    glDisableVertexAttribArray(program->aColor);
}

void GLBuffer_DrawElements(GLBuffer * const self)
{
    glDrawElements(GL_TRIANGLES, self->indicesSize, GL_UNSIGNED_INT, NULL);
}
