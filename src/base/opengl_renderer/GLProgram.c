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

#include "GLProgram.h"

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct GLProgram
{
    GLuint m_lastProgram;
    GLProgramLocation m_programs[_Type_size];
};

static void CheckProgram(GLuint obj);
static void CheckShader(GLuint obj);
static GLuint CreateProgram(GLProgramLocation_Type type);
static void CompileShader(GLuint program, GLenum type, const char *src);

static char *GetShaderSource(GLProgramLocation_Type type, const char *source)
{
    char src[1024];

    strcpy(src, "#version 100\n"); // OpenGL ES 2.0 / WebGL 1.0

    if (type == Type_Texture)
        strcat(src, "#define hasTexture 1\n");
    else if (type == Type_TextureBGRA)
        strcat(src, "#define hasTexture 1\n#define hasTextureBGRA 1\n");
    else
        strcat(src, "#define hasTexture 0\n");

    int size = strlen(source) + strlen(src) + 1;

    char *buffer = malloc(size);
    snprintf(buffer, size, "%s%s", src, source);

    return buffer;
}

static char *GetVertexShaderSource(GLProgramLocation_Type type)
{
    #include "shaders/shader.vert.h"

    return GetShaderSource(type, vert);
}

static char *GetFragmentShaderSource(GLProgramLocation_Type type)
{
    #include "shaders/shader.frag.h"

    return GetShaderSource(type, frag);
}

GLProgram *GLProgram_New()
{
    GLProgram * const self = malloc(sizeof (GLProgram));

    self->m_lastProgram = 0;

    return self;
}

void GLProgram_Delete(GLProgram * const self)
{
    if (!self)
        return;

    for (size_t i = 0; i < _Type_size; ++i)
    {
        GLProgramLocation program = self->m_programs[i];

        glDisableVertexAttribArray(program.aPosition);
        glDeleteProgram(program.program);
    }

    free(self);
}

const GLProgramLocation *GLProgram_InitProgram(GLProgram * const self, GLProgramLocation_Type type)
{
    GLuint program = CreateProgram(type);

    self->m_programs[type] = (GLProgramLocation) {
        .program = program,
        .aPosition = glGetAttribLocation(program, "aPosition"),
        .aUV = -1,
        .aColor = -1,
        .uProjection = glGetUniformLocation(program, "uProjection"),
        .uSampler = -1,
        .uSourcePosition = -1,
        .uTransform = glGetUniformLocation(program, "uTransform"),
    };

    if (type == Type_Texture || type == Type_TextureBGRA)
    {
        self->m_programs[type].aUV = glGetAttribLocation(program, "aUV");
        self->m_programs[type].uSourcePosition = glGetUniformLocation(program, "uSourcePosition");
        self->m_programs[type].uSampler = glGetUniformLocation(program, "uSampler");
    }
    else
    {
        self->m_programs[type].aColor = glGetAttribLocation(program, "aColor");
    }

    return &self->m_programs[type];
}

const GLProgramLocation *GLProgram_GetProgram(GLProgram * const self, GLProgramLocation_Type type)
{
    const GLProgramLocation *program = &self->m_programs[type];

    if (program->program != self->m_lastProgram)
    {
        glUseProgram(program->program);
        self->m_lastProgram = program->program;
    }

    return program;
}

GLuint CreateProgram(GLProgramLocation_Type type)
{
    GLuint program = glCreateProgram();

    char *vert = GetVertexShaderSource(type);
    char *frag = GetFragmentShaderSource(type);

    CompileShader(program, GL_VERTEX_SHADER, vert);
    CompileShader(program, GL_FRAGMENT_SHADER, frag);

    free(vert);
    free(frag);

    glLinkProgram(program);
    CheckProgram(program);
    glUseProgram(program);

    return program;
}

void CompileShader(GLuint program, GLenum type, const char *src)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    CheckShader(shader);

    glAttachShader(program, shader);
    glDeleteShader(shader);
}

void CheckProgram(GLuint obj)
{
    GLint status = GL_FALSE;
    char log[2048];

    glGetProgramiv(obj, GL_LINK_STATUS, &status);

    if (status == GL_TRUE)
        return;

    glGetProgramInfoLog(obj, sizeof(log), NULL, log);

    puts(log);
    exit(EXIT_FAILURE);
}

void CheckShader(GLuint obj)
{
    GLint status = GL_FALSE;
    char log[2048];

    glGetShaderiv(obj, GL_COMPILE_STATUS, &status);

    if (status == GL_TRUE)
        return;

    glGetShaderInfoLog(obj, sizeof(log), NULL, log);

    puts(log);
    exit(EXIT_FAILURE);
}
