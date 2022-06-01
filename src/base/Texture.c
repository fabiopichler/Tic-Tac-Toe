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

#include "Texture.h"
#include "Box.h"
#include "DataZipFile.h"
#include "rect.h"

#include "malloc.h"
#include "opengl_renderer/OpenGLRenderer.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>

struct Texture
{
    OpenGLRenderer *renderer;
    Texture2D *texture;
    int w;
    int h;

    Box *box;
    char *text;
    TTF_Font *font;
    int fontSize;
    bool reloadFont;
    Color textColor;

    IRect srcrect;
    double angle;
};

bool Texture_CreateTexture(Texture * const self, SDL_Surface *surface, TextureFilter filter);

Texture *Texture_New(OpenGLRenderer *renderer)
{
    Texture * const self = malloc(sizeof (Texture));

    self->renderer = renderer;
    self->texture = NULL;
    self->w = 0;
    self->h = 0;

    self->box = Box_New(0.f, 0.f, 0.f, 0.f);
    self->text = NULL;
    self->font = NULL;
    self->fontSize = 16;
    self->reloadFont = false;
    self->textColor = (Color) {60, 60, 60, 255};

    self->srcrect = (IRect) {0, 0, 0, 0};
    self->angle = 0.0;

    return self;
}

void Texture_Delete(Texture * const self)
{
    if (!self)
        return;

    Box_Delete(self->box);

    OpenGLRenderer_DestroyTexture(self->renderer, self->texture);
    TTF_CloseFont(self->font);

    free(self->text);
    free(self);
}

bool Texture_LoadImageFromFile(Texture * const self, const char *fileName, TextureFilter filter)
{
#ifdef USE_DATA_ZIP
    SDL_Surface *surface = IMG_Load_RW(DataZipFile_Load_RW(fileName), 1);
#else
    SDL_Surface *surface = IMG_Load(fileName);
#endif

    return Texture_CreateTexture(self, surface, filter);
}

bool Texture_MakeText(Texture * const self)
{
    if (!self->font || self->reloadFont)
    {
        TTF_CloseFont(self->font);

#ifdef USE_DATA_ZIP
        if (!(self->font = TTF_OpenFontRW(DataZipFile_Load_RW("fonts/NotoSans-Bold.ttf"), 1, self->fontSize)))
#else
        if (!(self->font = TTF_OpenFont("fonts/NotoSans-Bold.ttf", self->fontSize)))
#endif
        {
            printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
            return false;
        }

        self->reloadFont = false;
    }

    SDL_Color color = { self->textColor.r, self->textColor.g, self->textColor.b, self->textColor.a };
    SDL_Surface *text = TTF_RenderUTF8_Blended(self->font, self->text, color);
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, text->w, text->h, 32, SDL_PIXELFORMAT_BGRA32);

    SDL_BlitSurface(text, NULL, surface, NULL);
    SDL_FreeSurface(text);

    return Texture_CreateTexture(self, surface, Linear);
}

void Texture_SetText(Texture * const self, const char *text)
{
    free(self->text);

    const size_t size = strlen(text) + 1;
    self->text = malloc(size);

    memcpy(self->text, text, size);
}

void Texture_SetTextSize(Texture * const self, int ptsize)
{
    if (self->fontSize != ptsize)
    {
        self->fontSize = ptsize;
        self->reloadFont = true;
    }
}

void Texture_SetTextColor(Texture * const self, const Color *color)
{
    if (color)
        self->textColor = *color;
    else
        self->textColor = (Color) {60, 60, 60, 255};
}

void Texture_SetTextColorRGB(Texture * const self, uint8_t r, uint8_t g, uint8_t b)
{
    self->textColor.r = r;
    self->textColor.g = g;
    self->textColor.b = b;
    self->textColor.a = 255;
}

void Texture_SetTextColorRGBA(Texture * const self, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    self->textColor.r = r;
    self->textColor.g = g;
    self->textColor.b = b;
    self->textColor.a = a;
}

void Texture_SetSourceRect(Texture * const self, IRect srcrect)
{
    self->srcrect = srcrect;
}

void Texture_SetAngle(Texture * const self, double angle)
{
    self->angle = angle;
}

void Texture_Draw(Texture * const self)
{
    if (self->texture)
        OpenGLRenderer_Draw(self->renderer, self->texture, &self->srcrect, Box_Rect(self->box), self->angle);
}

bool Texture_CreateTexture(Texture * const self, SDL_Surface *surface, TextureFilter filter)
{
    if (surface)
    {
        OpenGLRenderer_DestroyTexture(self->renderer, self->texture);

        Image image = {
            .width = surface->w,
            .height = surface->h,
            .bytesPerPixel = surface->format->BytesPerPixel,
            .rmask = surface->format->Rmask,
            .pixels = surface->pixels,
        };

        self->texture = OpenGLRenderer_CreateTexture(self->renderer, &image, filter);

        if (self->texture)
        {
            self->w = surface->w;
            self->h = surface->h;
            self->srcrect.w = surface->w;
            self->srcrect.h = surface->h;
            Box_SetSize(self->box, surface->w, surface->h);
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

int Texture_GetWidth(Texture * const self)
{
    return self->w;
}

int Texture_GetHeight(Texture * const self)
{
    return self->h;
}

Box *Texture_Box(Texture * const self)
{
    return self->box;
}
