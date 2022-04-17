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

#include <SDL2/SDL_image.h>

#include <stdio.h>

struct Window
{
    SDL_Window *window;
    SDL_Rect rect;
};

Window *Window_New(int width, int height, const char *title)
{
    Window * const self = malloc(sizeof (Window));

    self->rect = (SDL_Rect) { .x = 0, .y = 0, .w = width, .h = height };

    self->window = SDL_CreateWindow(title,
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                self->rect.w,
                                self->rect.h,
                                SDL_WINDOW_SHOWN);

    if (!self->window)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }

    return self;
}

void Window_Delete(Window * const self)
{
    if (!self)
        return;

    SDL_DestroyWindow(self->window);

    free(self);
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

SDL_Rect Window_GetRect(Window * const self)
{
    return self->rect;
}
