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
    const int width;
    const int player1_y;
    const int player1Win_y;
    const int player2_y;
    const int player2Win_y;
    const int tied_y;
    const int tiedCount_y;
    const SDL_Rect sidebarRect;
    const SDL_Rect verticalLine;
    const SDL_Rect horizontalLine1;
    const SDL_Rect horizontalLine2;
    const SDL_Color textColor;

    Texture *player1Text;
    Texture *player1WinText;
    Texture *player2Text;
    Texture *player2WinText;
    Texture *tiedText;
    Texture *tiedCountText;
};

void Sidebar_SetupSizes(Sidebar *const this);
void Sidebar_CreateTextures(Sidebar *const this);
void Sidebar_UpdateTextRect(Sidebar *const this, Texture *texture, int y);
void Sidebar_UpdateText(Sidebar *const this, Texture *texture, int pos_y, int count);

Sidebar *Sidebar_New(SDL_Renderer *renderer, SceneGameRect *rect)
{
    Sidebar *const this = malloc(sizeof (Sidebar));

    this->renderer = renderer;
    this->rect = rect;

    Sidebar_SetupSizes(this);
    Sidebar_CreateTextures(this);

    return this;
}

void Sidebar_Delete(Sidebar *const this)
{
    if (!this)
        return;

    Texture_Delete(this->player1Text);
    Texture_Delete(this->player1WinText);
    Texture_Delete(this->player2Text);
    Texture_Delete(this->player2WinText);
    Texture_Delete(this->tiedText);
    Texture_Delete(this->tiedCountText);
    free(this);
}

void Sidebar_Draw(Sidebar *const this)
{
    SDL_SetRenderDrawColor(this->renderer, 230, 240, 240, 255);
    SDL_RenderFillRect(this->renderer, &this->sidebarRect);

    SDL_SetRenderDrawColor(this->renderer, 80, 160, 160, 255);
    SDL_RenderFillRect(this->renderer, &this->verticalLine);
    SDL_RenderFillRect(this->renderer, &this->horizontalLine1);
    SDL_RenderFillRect(this->renderer, &this->horizontalLine2);

    Texture_Draw(this->player1Text, NULL, NULL);
    Texture_Draw(this->player1WinText, NULL, NULL);
    Texture_Draw(this->player2Text, NULL, NULL);
    Texture_Draw(this->player2WinText, NULL, NULL);
    Texture_Draw(this->tiedText, NULL, NULL);
    Texture_Draw(this->tiedCountText, NULL, NULL);
}

void Sidebar_SetPlayer1WinText(Sidebar *const this, int count)
{
    Sidebar_UpdateText(this, this->player1WinText, this->player1Win_y, count);
}

void Sidebar_SetPlayer2WinText(Sidebar *const this, int count)
{
    Sidebar_UpdateText(this, this->player2WinText, this->player2Win_y, count);
}

void Sidebar_SetTiedCountText(Sidebar *const this, int count)
{
    Sidebar_UpdateText(this, this->tiedCountText, this->tiedCount_y, count);
}

void Sidebar_SetupSizes(Sidebar *const this)
{
    const int border_w = 2;
    const int title_margin = 30;
    const int number_margin = 75;
    const int second_block = this->rect->sidebar_h / 3;
    const int third_block = second_block * 2;

    *(int *)&this->width = this->rect->sidebar_w - border_w;

    *(int *)&this->player1_y = title_margin;
    *(int *)&this->player1Win_y = number_margin;
    *(int *)&this->player2_y = second_block + title_margin;
    *(int *)&this->player2Win_y = second_block + number_margin;
    *(int *)&this->tied_y = third_block + title_margin;
    *(int *)&this->tiedCount_y = third_block + number_margin;

    *(SDL_Rect *)&this->sidebarRect = (SDL_Rect) {0, 0, this->width, this->rect->sidebar_h};
    *(SDL_Rect *)&this->verticalLine = (SDL_Rect) {this->width, 0, border_w, this->rect->sidebar_h};
    *(SDL_Rect *)&this->horizontalLine1 = (SDL_Rect) {0, second_block - border_w, this->width, border_w};
    *(SDL_Rect *)&this->horizontalLine2 = (SDL_Rect) {0, third_block - border_w, this->width, border_w};

    *(SDL_Color *)&this->textColor = (SDL_Color) {50, 140, 140, 255};
}

void Sidebar_CreateTextures(Sidebar *const this)
{
    this->player1Text = Texture_New(this->renderer);
    this->player1WinText = Texture_New(this->renderer);
    this->player2Text = Texture_New(this->renderer);
    this->player2WinText = Texture_New(this->renderer);
    this->tiedText = Texture_New(this->renderer);
    this->tiedCountText = Texture_New(this->renderer);

    Texture_SetupText(this->player1Text, "Vitórias do jogador 1", 16, &this->textColor);
    Texture_SetupText(this->player1WinText, "0", 40, &this->textColor);
    Texture_SetupText(this->player2Text, "Vitórias do jogador 2", 16, &this->textColor);
    Texture_SetupText(this->player2WinText, "0", 40, &this->textColor);
    Texture_SetupText(this->tiedText, "Total de empates", 16, &this->textColor);
    Texture_SetupText(this->tiedCountText, "0", 40, &this->textColor);

    Texture_MakeText(this->player1Text);
    Texture_MakeText(this->player1WinText);
    Texture_MakeText(this->player2Text);
    Texture_MakeText(this->player2WinText);
    Texture_MakeText(this->tiedText);
    Texture_MakeText(this->tiedCountText);

    Sidebar_UpdateTextRect(this, this->player1Text, this->player1_y);
    Sidebar_UpdateTextRect(this, this->player1WinText, this->player1Win_y);
    Sidebar_UpdateTextRect(this, this->player2Text, this->player2_y);
    Sidebar_UpdateTextRect(this, this->player2WinText, this->player2Win_y);
    Sidebar_UpdateTextRect(this, this->tiedText, this->tied_y);
    Sidebar_UpdateTextRect(this, this->tiedCountText, this->tiedCount_y);
}

void Sidebar_UpdateTextRect(Sidebar *const this, Texture *texture, int y)
{
    (void)this;

    int w = Texture_GetWidth(texture);
    int h = Texture_GetHeight(texture);

    Texture_SetRect(texture, &(SDL_Rect) {
                        .x = (this->width - w) / 2,
                        .y = y,
                        .w = w,
                        .h = h
                    });
}

void Sidebar_UpdateText(Sidebar *const this, Texture *texture, int pos_y, int count)
{
    char text[6];
    snprintf(text, sizeof (text), "%d", count);
    Texture_SetText(texture, text);
    Texture_MakeText(texture);
    Sidebar_UpdateTextRect(this, texture, pos_y);
}
