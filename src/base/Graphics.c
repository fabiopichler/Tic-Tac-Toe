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

#include "Graphics.h"
#include "rect.h"
#include "opengl_renderer/OpenGLRenderer.h"

#include <stdio.h>

#include <SDL2/SDL.h>

struct Graphics
{
    OpenGLRenderer *renderer;
};

Graphics *Graphics_New(Window *window)
{
    Graphics * const self = malloc(sizeof (Graphics));

    self->renderer = OpenGLRenderer_New();

    if (!self->renderer)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(Window_GetSDLWindow(window));
        SDL_Quit();
        exit(-1);
    }

    IRect rect = Window_GetRect(window);
    SetRenderLogicalSize(self, rect.w, rect.h);

    return self;
}

void Graphics_Delete(Graphics * const self)
{
    if (!self)
        return;

    OpenGLRenderer_Delete(self->renderer);

    free(self);
}

OpenGLRenderer *Graphics_GetRenderer(Graphics * const self)
{
    return self->renderer;
}

int SetRenderLogicalSize(Graphics * const self, int w, int h)
{
    OpenGLRenderer_SetLogicalSize(self->renderer, w, h);
    OpenGLRenderer_SetViewportSize(self->renderer, w, h);

    return 1;
}
