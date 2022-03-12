//-------------------------------------------------------------------------------
// Copyright (c) 2020 Fábio Pichler
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

#include <SDL2/SDL_image.h>

#include <stdio.h>

struct Window
{
    SDL_Window *window;
    SDL_Rect rect;
};

Window *Window_New(int width, int height)
{
    Window *const self = malloc(sizeof (Window));

    self->rect = (SDL_Rect) { .x = 0, .y = 0, .w = width, .h = height };

    self->window = SDL_CreateWindow("Tic Tac Toe",
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

    SDL_Surface *icon = IMG_Load("images/window-icon.png");
    SDL_SetWindowIcon(self->window, icon);
    SDL_FreeSurface(icon);

    return self;
}

void Window_Delete(Window *const self)
{
    if (!self)
        return;

    SDL_DestroyWindow(self->window);
    free(self);
}

SDL_Window *Window_GetSDLWindow(Window *const self)
{
    return self->window;
}

SDL_Rect Window_GetRect(Window *const self)
{
    return self->rect;
}
