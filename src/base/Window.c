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

struct Window
{
    SDL_Window *window;
    SDL_Rect rect;
};

Window *Window_New(int width, int height)
{
    Window *const this = malloc(sizeof (Window));

    this->rect = (SDL_Rect) { .x = 0, .y = 0, .w = width, .h = height };

    this->window = SDL_CreateWindow("Tic Tac Toe",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                this->rect.w,
                                this->rect.h,
                                SDL_WINDOW_SHOWN);

    if (!this->window)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }

    SDL_Surface *icon = IMG_Load("images/window-icon.png");
    SDL_SetWindowIcon(this->window, icon);
    SDL_FreeSurface(icon);

    return this;
}

void Window_Delete(Window *const this)
{
    if (!this)
        return;

    SDL_DestroyWindow(this->window);
    free(this);
}

SDL_Window *Window_GetSDLWindow(Window *const this)
{
    return this->window;
}

SDL_Rect Window_GetRect(Window *const this)
{
    return this->rect;
}
