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

#include "Sidebar.h"
#include "../base/Texture.h"

#include <malloc.h>
#include <stdio.h>

struct Sidebar
{
    SDL_Renderer *renderer;
    const SceneGameRect *rect;
    int width;
    int player1_y;
    int player1Win_y;
    int player2_y;
    int player2Win_y;
    int tied_y;
    int tiedCount_y;
    SDL_Rect sidebarRect;
    SDL_Rect verticalLine;
    SDL_Rect horizontalLine1;
    SDL_Rect horizontalLine2;
    SDL_Color textColor;

    Texture *player1Text;
    Texture *player1WinText;
    Texture *player2Text;
    Texture *player2WinText;
    Texture *tiedText;
    Texture *tiedCountText;
};

void Sidebar_SetupSizes(Sidebar *const self);
void Sidebar_CreateTextures(Sidebar *const self);
void Sidebar_UpdateTextRect(Sidebar *const self, Texture *texture, int y);
void Sidebar_UpdateText(Sidebar *const self, Texture *texture, int pos_y, int count);

Sidebar *Sidebar_New(SDL_Renderer *renderer, SceneGameRect *rect)
{
    Sidebar *const self = malloc(sizeof (Sidebar));

    self->renderer = renderer;
    self->rect = rect;

    Sidebar_SetupSizes(self);
    Sidebar_CreateTextures(self);

    return self;
}

void Sidebar_Delete(Sidebar *const self)
{
    if (!self)
        return;

    Texture_Delete(self->player1Text);
    Texture_Delete(self->player1WinText);
    Texture_Delete(self->player2Text);
    Texture_Delete(self->player2WinText);
    Texture_Delete(self->tiedText);
    Texture_Delete(self->tiedCountText);
    free(self);
}

void Sidebar_Draw(Sidebar *const self)
{
    SDL_SetRenderDrawColor(self->renderer, 230, 240, 240, 255);
    SDL_RenderFillRect(self->renderer, &self->sidebarRect);

    SDL_SetRenderDrawColor(self->renderer, 80, 160, 160, 255);
    SDL_RenderFillRect(self->renderer, &self->verticalLine);
    SDL_RenderFillRect(self->renderer, &self->horizontalLine1);
    SDL_RenderFillRect(self->renderer, &self->horizontalLine2);

    Texture_Draw(self->player1Text, NULL, NULL);
    Texture_Draw(self->player1WinText, NULL, NULL);
    Texture_Draw(self->player2Text, NULL, NULL);
    Texture_Draw(self->player2WinText, NULL, NULL);
    Texture_Draw(self->tiedText, NULL, NULL);
    Texture_Draw(self->tiedCountText, NULL, NULL);
}

void Sidebar_SetPlayer1WinText(Sidebar *const self, int count)
{
    Sidebar_UpdateText(self, self->player1WinText, self->player1Win_y, count);
}

void Sidebar_SetPlayer2WinText(Sidebar *const self, int count)
{
    Sidebar_UpdateText(self, self->player2WinText, self->player2Win_y, count);
}

void Sidebar_SetTiedCountText(Sidebar *const self, int count)
{
    Sidebar_UpdateText(self, self->tiedCountText, self->tiedCount_y, count);
}

void Sidebar_SetupSizes(Sidebar *const self)
{
    const int border_w = 2;
    const int title_margin = 30;
    const int number_margin = 75;
    const int second_block = self->rect->sidebar_h / 3;
    const int third_block = second_block * 2;

    self->width = self->rect->sidebar_w - border_w;

    self->player1_y = title_margin;
    self->player1Win_y = number_margin;
    self->player2_y = second_block + title_margin;
    self->player2Win_y = second_block + number_margin;
    self->tied_y = third_block + title_margin;
    self->tiedCount_y = third_block + number_margin;

    self->sidebarRect = (SDL_Rect) {0, 0, self->width, self->rect->sidebar_h};
    self->verticalLine = (SDL_Rect) {self->width, 0, border_w, self->rect->sidebar_h};
    self->horizontalLine1 = (SDL_Rect) {0, second_block - border_w, self->width, border_w};
    self->horizontalLine2 = (SDL_Rect) {0, third_block - border_w, self->width, border_w};

    self->textColor = (SDL_Color) {50, 140, 140, 255};
}

void Sidebar_CreateTextures(Sidebar *const self)
{
    self->player1Text = Texture_New(self->renderer);
    self->player1WinText = Texture_New(self->renderer);
    self->player2Text = Texture_New(self->renderer);
    self->player2WinText = Texture_New(self->renderer);
    self->tiedText = Texture_New(self->renderer);
    self->tiedCountText = Texture_New(self->renderer);

    Texture_SetupText(self->player1Text, "Vitórias do jogador 1", 16, &self->textColor);
    Texture_SetupText(self->player1WinText, "0", 40, &self->textColor);
    Texture_SetupText(self->player2Text, "Vitórias do jogador 2", 16, &self->textColor);
    Texture_SetupText(self->player2WinText, "0", 40, &self->textColor);
    Texture_SetupText(self->tiedText, "Total de empates", 16, &self->textColor);
    Texture_SetupText(self->tiedCountText, "0", 40, &self->textColor);

    Texture_MakeText(self->player1Text);
    Texture_MakeText(self->player1WinText);
    Texture_MakeText(self->player2Text);
    Texture_MakeText(self->player2WinText);
    Texture_MakeText(self->tiedText);
    Texture_MakeText(self->tiedCountText);

    Sidebar_UpdateTextRect(self, self->player1Text, self->player1_y);
    Sidebar_UpdateTextRect(self, self->player1WinText, self->player1Win_y);
    Sidebar_UpdateTextRect(self, self->player2Text, self->player2_y);
    Sidebar_UpdateTextRect(self, self->player2WinText, self->player2Win_y);
    Sidebar_UpdateTextRect(self, self->tiedText, self->tied_y);
    Sidebar_UpdateTextRect(self, self->tiedCountText, self->tiedCount_y);
}

void Sidebar_UpdateTextRect(Sidebar *const self, Texture *texture, int y)
{
    (void)self;

    int w = Texture_GetWidth(texture);
    int h = Texture_GetHeight(texture);

    Texture_SetRect(texture, &(SDL_Rect) {
                        .x = (self->width - w) / 2,
                        .y = y,
                        .w = w,
                        .h = h
                    });
}

void Sidebar_UpdateText(Sidebar *const self, Texture *texture, int pos_y, int count)
{
    char text[6];
    snprintf(text, sizeof (text), "%d", count);
    Texture_SetText(texture, text);
    Texture_MakeText(texture);
    Sidebar_UpdateTextRect(self, texture, pos_y);
}
