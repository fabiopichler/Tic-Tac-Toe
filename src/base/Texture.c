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

bool Texture_CreateTexture(Texture *const this, SDL_Surface *surface);

Texture *Texture_New(SDL_Renderer *renderer)
{
    Texture *const this = malloc(sizeof (Texture));

    this->renderer = renderer;
    this->texture = NULL;
    this->rect = (SDL_Rect) {0, 0, 0, 0};
    this->w = 0;
    this->h = 0;

    this->text = NULL;
    this->font = NULL;
    this->fontSize = 16;
    this->reloadFont = false;
    this->textColor = (SDL_Color) {60, 60, 60, 255};

    return this;
}

void Texture_Delete(Texture *const this)
{
    if (!this)
        return;

    SDL_DestroyTexture(this->texture);
    TTF_CloseFont(this->font);
    free(this->text);
    free(this);
}

bool Texture_LoadImageFromFile(Texture *const this, const char *fileName)
{
    return Texture_CreateTexture(this, IMG_Load(fileName));
}

bool Texture_MakeText(Texture *const this)
{
    if (!this->font || this->reloadFont)
    {
        TTF_CloseFont(this->font);

        if (!(this->font = TTF_OpenFont("fonts/NotoSans-Bold.ttf", this->fontSize)))
        {
            printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
            return false;
        }

        this->reloadFont = false;
    }

    SDL_Surface *surface = TTF_RenderUTF8_Blended(this->font, this->text, this->textColor);

    return Texture_CreateTexture(this, surface);
}

void Texture_SetupText(Texture *const this, const char *text, int ptsize, SDL_Color color)
{
    Texture_SetText(this, text);
    Texture_SetTextSize(this, ptsize);
    Texture_SetTextColor(this, color);
}

void Texture_SetText(Texture *const this, const char *text)
{
    free(this->text);

    size_t size = strlen(text) + 1;
    this->text = malloc(size);

    memcpy(this->text, text, size);
}

void Texture_SetTextSize(Texture *const this, int ptsize)
{
    if (this->fontSize != ptsize)
    {
        this->fontSize = ptsize;
        this->reloadFont = true;
    }
}

void Texture_SetTextColor(Texture *const this, SDL_Color color)
{
    this->textColor = color;
}

void Texture_Draw(Texture *const this, const SDL_Rect *srcrect, const SDL_Rect *dstrect)
{
    if (this->texture)
        SDL_RenderCopy(this->renderer, this->texture, srcrect, dstrect ? dstrect : &this->rect);
}

void Texture_DrawEx(Texture *const this, const SDL_Rect *srcrect, const SDL_Rect *dstrect, const double angle)
{
    if (this->texture)
        SDL_RenderCopyEx(this->renderer, this->texture, srcrect, dstrect ? dstrect : &this->rect, angle, NULL, SDL_FLIP_NONE);
}

bool Texture_CreateTexture(Texture *const this, SDL_Surface *surface)
{
    if (surface)
    {
        if (this->texture)
            SDL_DestroyTexture(this->texture);
            //SDL_UpdateTexture(this->texture, NULL, surface->pixels, surface->pitch);
        //else

        this->texture = SDL_CreateTextureFromSurface(this->renderer, surface);

        if (this->texture)
        {
            this->w = surface->w;
            this->h = surface->h;
        }
        else
        {
            printf("Unable to update texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }

        SDL_FreeSurface(surface);
    }
    else
    {
        this->texture = NULL;
        printf("Unable to render surface! SDL Error: %s\n", TTF_GetError());
    }

    return this->texture != NULL;
}

void Texture_SetPos(Texture *const this, int x, int y)
{
    this->rect.x = x;
    this->rect.x = y;
}

void Texture_SetRect(Texture *const this, SDL_Rect rect)
{
    this->rect = rect;
}

int Texture_GetWidth(Texture *const this)
{
    return this->w;
}

int Texture_GetHeight(Texture *const this)
{
    return this->h;
}
