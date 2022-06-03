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

#include "Window.h"
#include "DataZipFile.h"
#include "rect.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdbool.h>

struct Window
{
    SDL_Window *window;
    SDL_GLContext context;
    IRect rect;
};

typedef struct OpenGLList
{
    const char *name;
    int profile;
    int majorVersion;
    int minorVersion;
} OpenGLList;

static const OpenGLList glList[] = {
#ifdef RENDERER_GLES
    { "OpenGL ES 3.0", SDL_GL_CONTEXT_PROFILE_ES, 3, 0 },
    { "OpenGL ES 2.0", SDL_GL_CONTEXT_PROFILE_ES, 2, 0 },
#else
    { "OpenGL 3.3 (Core Profile)", SDL_GL_CONTEXT_PROFILE_CORE, 3, 3 },
    { "OpenGL 2.1", SDL_GL_CONTEXT_PROFILE_COMPATIBILITY, 2, 1 },
#endif
    { NULL, 0, 0, 0 },
};

static bool CreateGLContext(Window * const self);

Window *Window_New(int width, int height, const char *title)
{
    Window * const self = malloc(sizeof (Window));

    self->rect = (IRect) { .x = 0, .y = 0, .w = width, .h = height };
    self->context = NULL;

    self->window = SDL_CreateWindow(title,
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                self->rect.w,
                                self->rect.h,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (!self->window)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }

    if (!CreateGLContext(self))
    {
        Window_Delete(self);
        return NULL;
    }

    return self;
}

void Window_Delete(Window * const self)
{
    if (!self)
        return;

    SDL_GL_DeleteContext(self->context);
    SDL_DestroyWindow(self->window);

    free(self);
}

bool CreateGLContext(Window * const self)
{
    for (const OpenGLList *gl = glList; gl->name != NULL; ++gl)
    {
        printf("Creating the %s context... ", gl->name);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gl->profile);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl->majorVersion);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl->minorVersion);

        self->context = SDL_GL_CreateContext(self->window);

        if (self->context)
        {
            printf("Ok.\n");

            SDL_GL_MakeCurrent(self->window, self->context);
            SDL_GL_SetSwapInterval(1);

            return true;
        }
        else
        {
            printf("Error.\nUnable to create %s context.\n", gl->name);
        }
    }

    return false;
}

void Window_SetWindowIcon(Window * const self, const char *filename)
{
#ifdef USE_DATA_ZIP
    SDL_Surface *surface = IMG_Load_RW(DataZipFile_Load_RW(filename), 1);
#else
    SDL_Surface *surface = IMG_Load(filename);
#endif
    SDL_SetWindowIcon(self->window, surface);
    SDL_FreeSurface(surface);
}

void Window_SetWindowTitle(Window * const self, const char *title)
{
    SDL_SetWindowTitle(self->window, title);
}

SDL_Window *Window_GetSDLWindow(Window * const self)
{
    return self->window;
}

IRect Window_GetRect(Window * const self)
{
    return self->rect;
}

void Window_SwapWindow(Window * const self)
{
    SDL_GL_SwapWindow(self->window);
}
