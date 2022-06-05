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

#include "OpenGLRenderer.h"
#include "GLBuffer.h"
#include "GLProgram.h"
#include "GLTexture.h"
#include "../rect.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

#include <cglm/cam.h>

#include <SDL2/SDL_video.h>

struct OpenGLRenderer
{
    GLProgram *m_program;
    GLBuffer *m_buffer;
    GLTexture *m_texture;
    Vec2 m_viewport;
    Vec2 m_logical;
};

static void UpdateProjection(OpenGLRenderer * const self, GLint uProjection);
static void ColorToArray(const Color *color, vec4 array[4]);

OpenGLRenderer *OpenGLRenderer_New()
{
    OpenGLRenderer * const self = malloc(sizeof (OpenGLRenderer));

    self->m_program = GLProgram_New();
    self->m_buffer = GLBuffer_New();
    self->m_texture = GLTexture_New();

    self->m_viewport = (Vec2) { 0.0f, 0.0f };
    self->m_logical = (Vec2) { 0.0f, 0.0f };

    OpenGLRenderer_InitGL(self);

    return self;
}

void OpenGLRenderer_Delete(OpenGLRenderer * const self)
{
    if (!self)
        return;

    GLBuffer_Delete(self->m_buffer);
    GLProgram_Delete(self->m_program);
    GLTexture_Delete(self->m_texture);

    free(self);
}

void OpenGLRenderer_InitGL(OpenGLRenderer * const self)
{
#ifdef RENDERER_GL_ES
    int version = gladLoadGLES2((GLADloadfunc) SDL_GL_GetProcAddress);
#else
    int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);
#endif

    if (!version)
    {
        puts("Failed to initialize the OpenGL context.");
        exit(EXIT_FAILURE);
    }

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        printf("OpenGL Error: %d\n", error);

        if (error != 1280)
            exit(EXIT_FAILURE);
    }

#ifdef RENDERER_GL_ES
    printf("Game renderer: OpenGL ES ");
#else
    printf("Game renderer: OpenGL ");
#endif
    printf("%d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    printf("GL renderer: %s\n", glGetString(GL_RENDERER));
    printf("GL version: %s\n", glGetString(GL_VERSION));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLBuffer_Init(self->m_buffer);
    GLTexture_Init(self->m_texture);

    for (size_t i = 0; i < _Type_size; ++i)
    {
        const GLProgramLocation *program = GLProgram_InitProgram(self->m_program, i);
        UpdateProjection(self, program->uProjection);
    }
}

Texture2D *OpenGLRenderer_CreateTexture(OpenGLRenderer * const self, const Image *image, TextureFilter filter)
{
    return GLTexture_CreateTexture(self->m_texture, image, filter);
}

void OpenGLRenderer_DestroyTexture(OpenGLRenderer * const self, Texture2D *texture)
{
    GLTexture_DestroyTexture(self->m_texture, texture);
}

void OpenGLRenderer_Clear(OpenGLRenderer * const self)
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderer_Draw(OpenGLRenderer * const self, const Texture2D *texture, const IRect *srcrect, const Rect *dstrect, const float angle)
{
    if (!texture)
        return;

    mat4 matrix;
    glm_mat4_identity(matrix);

    if (angle == 0.0f)
    {
        glm_translate(matrix, (vec3) { dstrect->x, dstrect->y, 0.0f });
    }
    else
    {
        const Vec2 center = { dstrect->w / 2.0f, dstrect->h / 2.0f };

        glm_translate(matrix, (vec3) { dstrect->x + center.x, dstrect->y + center.y, 0.0f });
        glm_rotate(matrix, glm_rad(angle), (vec3) { 0.0f, 0.0f, 1.0f });
        glm_translate(matrix, (vec3) { -center.x, -center.y, 0.0f });
    }

    glm_scale(matrix, (vec3) { dstrect->w, dstrect->h, 0.0f });

#ifdef RENDERER_GL_ES
    const GLProgramLocation *program = GLProgram_GetProgram(self->m_program,
                                                            texture->format == BGRA ? Type_TextureBGRA : Type_Texture);
#else
    const GLProgramLocation *program = GLProgram_GetProgram(self->m_program, Type_Texture);
#endif

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    if (srcrect)
    {
        glUniform4f(program->uSourcePosition,
                    srcrect->x / texture->width, srcrect->y / texture->height,
                    srcrect->w / texture->width, srcrect->h / texture->height);
    }

    glUniformMatrix4fv(program->uTransform, 1, false, matrix[0]);
    glUniform1i(program->uSampler, 0);

    GLBuffer_EnablePositionVBO(self->m_buffer, program);
    GLBuffer_DrawElements(self->m_buffer);
    GLBuffer_DisablePositionVBO(self->m_buffer, program);
}

void OpenGLRenderer_FillRect(OpenGLRenderer * const self, const Rect *rect, const Color *color)
{
    mat4 matrix;
    glm_mat4_identity(matrix);
    glm_translate(matrix, (vec3) { rect->x, rect->y, 0.0f });
    glm_scale(matrix, (vec3) { rect->w, rect->h, 0.0f });

    const GLProgramLocation *program = GLProgram_GetProgram(self->m_program, Type_Color);

    glUniformMatrix4fv(program->uTransform, 1, false, matrix[0]);

    vec4 colorArray[4];
    ColorToArray(color, colorArray);

    GLBuffer_EnablePositionVBO(self->m_buffer, program);
    GLBuffer_EnableColorVBO(self->m_buffer, program, colorArray);

    GLBuffer_DrawElements(self->m_buffer);

    GLBuffer_DisableColorVBO(self->m_buffer, program);
    GLBuffer_DisablePositionVBO(self->m_buffer, program);
}

void OpenGLRenderer_SetViewportSize(OpenGLRenderer * const self, int w, int h)
{
    self->m_viewport.x = w;
    self->m_viewport.y = h;

    int new_x, new_y, new_w, new_h;
    float want_aspect = self->m_logical.x / self->m_logical.y;
    float real_aspect = self->m_viewport.x / self->m_viewport.y;

    if (want_aspect > real_aspect)
    {
        float scale = self->m_viewport.x / self->m_logical.x;
        new_x = 0;
        new_w = self->m_viewport.x;
        new_h = floor(self->m_logical.y * scale);
        new_y = (self->m_viewport.y - new_h) / 2;
    }
    else
    {
        float scale = self->m_viewport.y / self->m_logical.y;
        new_y = 0;
        new_h = self->m_viewport.y;
        new_w = floor(self->m_logical.x * scale);
        new_x = (self->m_viewport.x - new_w) / 2;
    }

    glViewport(new_x, new_y, new_w, new_h);
}

void OpenGLRenderer_SetLogicalSize(OpenGLRenderer * const self, int w, int h)
{
    self->m_logical.x = w;
    self->m_logical.y = h;

    for (size_t i = 0; i < _Type_size; ++i)
        UpdateProjection(self, GLProgram_GetProgram(self->m_program, i)->uProjection);
}

void UpdateProjection(OpenGLRenderer * const self, GLint uProjection)
{
    mat4 proj, view, model, mvp;

    glm_ortho(0.0f, self->m_logical.x, self->m_logical.y, 0.0f, 1.0f, 0.0f, proj);
    glm_lookat((vec3) { 0.0f, 0.0f, 1.0f }, (vec3) {0.0f, 0.0f, 0.0f }, (vec3) {0.0f, 1.0f, 0.0f }, view);
    glm_mat4_identity(model);
    glm_mat4_mulN((mat4 *[]) { &proj, &view, &model }, 3, mvp);

    glUniformMatrix4fv(uProjection, 1, GL_FALSE, mvp[0]);
}

void ColorToArray(const Color *color, vec4 array[4])
{
    vec4 _array[4] = {
        { color->r, color->g, color->b, color->a },
        { color->r, color->g, color->b, color->a },
        { color->r, color->g, color->b, color->a },
        { color->r, color->g, color->b, color->a },
    };

    memcpy(array, _array, sizeof (vec4[4]));
}
