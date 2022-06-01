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
    GLuint m_vao;
    GLuint m_positionVBO, m_colorVBO, m_elementBuffer;
    int m_indicesSize;
};

GLBuffer *GLBuffer_New()
{
    GLBuffer * const self = malloc(sizeof (GLBuffer));

    self->m_vao = 0;
    self->m_positionVBO = 0;
    self->m_colorVBO = 0;
    self->m_elementBuffer = 0;
    self->m_indicesSize = 0;

    return self;
}

void GLBuffer_Delete(GLBuffer * const self)
{
    if (!self)
        return;

    glDeleteBuffers(1, &self->m_positionVBO);
    glDeleteBuffers(1, &self->m_elementBuffer);

#if !defined(RENDERER_GLES) && !defined(RENDERER_GL2)
    glDeleteVertexArrays(1, &self->m_vao);
#endif

    free(self);
}

void GLBuffer_Init(GLBuffer * const self)
{
#if !defined(RENDERER_GLES) && !defined(RENDERER_GL2)
    glGenVertexArrays(1, &self->m_vao);
    glBindVertexArray(self->m_vao);
#endif

    const Vertex vertices[4] = {
        { { 0.0f, 1.0f }, { 0.0f, 1.0f } },
        { { 1.0f, 1.0f }, { 1.0f, 1.0f } },
        { { 1.0f, 0.0f }, { 1.0f, 0.0f } },
        { { 0.0f, 0.0f }, { 0.0f, 0.0f } },
    };

    const ivec3 indices[2] = {
        { 0, 1, 2 },
        { 0, 2, 3 },
    };

    self->m_indicesSize = 6;

    glGenBuffers(1, &self->m_positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, self->m_positionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &self->m_elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &self->m_colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, self->m_colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4[4]), NULL, GL_DYNAMIC_DRAW);
}

void GLBuffer_EnablePositionVBO(GLBuffer * const self, const GLProgramLocation *program)
{
    glEnableVertexAttribArray(program->aPosition);
    glEnableVertexAttribArray(program->aUV);
    glBindBuffer(GL_ARRAY_BUFFER, self->m_positionVBO);
    glVertexAttribPointer(program->aPosition, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(program->aUV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, UV));
}

void GLBuffer_DisablePositionVBO(GLBuffer * const self, const GLProgramLocation *program)
{
    glDisableVertexAttribArray(program->aPosition);
    glDisableVertexAttribArray(program->aUV);
}

void GLBuffer_EnableColorVBO(GLBuffer * const self, const GLProgramLocation *program, const vec4 colors[4])
{
    glEnableVertexAttribArray(program->aColor);
    glBindBuffer(GL_ARRAY_BUFFER, self->m_colorVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4[4]), colors);
    glVertexAttribPointer(program->aColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void GLBuffer_DisableColorVBO(GLBuffer * const self, const GLProgramLocation *program)
{
    glDisableVertexAttribArray(program->aColor);
}

void GLBuffer_DrawElements(GLBuffer * const self)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_elementBuffer);
    glDrawElements(GL_TRIANGLES, self->m_indicesSize, GL_UNSIGNED_INT, NULL);
}