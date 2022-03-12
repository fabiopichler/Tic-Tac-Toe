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

#include "Texture.h"

#include "malloc.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>

struct Texture
{
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect rect;
    int w;
    int h;

    char *text;
    TTF_Font *font;
    int fontSize;
    bool reloadFont;
    SDL_Color textColor;
};

bool Texture_CreateTexture(Texture *const self, SDL_Surface *surface);

Texture *Texture_New(SDL_Renderer *renderer)
{
    Texture *const self = malloc(sizeof (Texture));

    self->renderer = renderer;
    self->texture = NULL;
    self->rect = (SDL_Rect) {0, 0, 0, 0};
    self->w = 0;
    self->h = 0;

    self->text = NULL;
    self->font = NULL;
    self->fontSize = 16;
    self->reloadFont = false;
    self->textColor = (SDL_Color) {60, 60, 60, 255};

    return self;
}

void Texture_Delete(Texture *const self)
{
    if (!self)
        return;

    SDL_DestroyTexture(self->texture);
    TTF_CloseFont(self->font);
    free(self->text);
    free(self);
}

bool Texture_LoadImageFromFile(Texture *const self, const char *fileName)
{
    return Texture_CreateTexture(self, IMG_Load(fileName));
}

bool Texture_MakeText(Texture *const self)
{
    if (!self->font || self->reloadFont)
    {
        TTF_CloseFont(self->font);

        if (!(self->font = TTF_OpenFont("fonts/NotoSans-Bold.ttf", self->fontSize)))
        {
            printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
            return false;
        }

        self->reloadFont = false;
    }

    SDL_Surface *surface = TTF_RenderUTF8_Blended(self->font, self->text, self->textColor);

    return Texture_CreateTexture(self, surface);
}

void Texture_SetupText(Texture *const self, const char *text, int ptsize, const SDL_Color *color)
{
    Texture_SetText(self, text);
    Texture_SetTextSize(self, ptsize);
    Texture_SetTextColor(self, color);
}

void Texture_SetText(Texture *const self, const char *text)
{
    free(self->text);

    size_t size = strlen(text) + 1;
    self->text = malloc(size);

    memcpy(self->text, text, size);
}

void Texture_SetTextSize(Texture *const self, int ptsize)
{
    if (self->fontSize != ptsize)
    {
        self->fontSize = ptsize;
        self->reloadFont = true;
    }
}

void Texture_SetTextColor(Texture *const self, const SDL_Color *color)
{
    if (color)
        self->textColor = *color;
}

void Texture_Draw(Texture *const self, const SDL_Rect *srcrect, const SDL_Rect *dstrect)
{
    if (self->texture)
        SDL_RenderCopy(self->renderer, self->texture, srcrect, dstrect ? dstrect : &self->rect);
}

void Texture_DrawEx(Texture *const self, const SDL_Rect *srcrect, const SDL_Rect *dstrect, const double angle)
{
    if (self->texture)
        SDL_RenderCopyEx(self->renderer, self->texture, srcrect, dstrect ? dstrect : &self->rect, angle, NULL, SDL_FLIP_NONE);
}

bool Texture_CreateTexture(Texture *const self, SDL_Surface *surface)
{
    if (surface)
    {
        if (self->texture)
            SDL_DestroyTexture(self->texture);
            //SDL_UpdateTexture(self->texture, NULL, surface->pixels, surface->pitch);
        //else

        self->texture = SDL_CreateTextureFromSurface(self->renderer, surface);

        if (self->texture)
        {
            self->w = surface->w;
            self->h = surface->h;
        }
        else
        {
            printf("Unable to update texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }

        SDL_FreeSurface(surface);
    }
    else
    {
        self->texture = NULL;
        printf("Unable to render surface! SDL Error: %s\n", TTF_GetError());
    }

    return self->texture != NULL;
}

void Texture_SetPos(Texture *const self, int x, int y)
{
    self->rect.x = x;
    self->rect.x = y;
}

void Texture_SetRect(Texture *const self, const SDL_Rect *rect)
{
    if (rect)
        self->rect = *rect;
}

int Texture_GetWidth(Texture *const self)
{
    return self->w;
}

int Texture_GetHeight(Texture *const self)
{
    return self->h;
}
