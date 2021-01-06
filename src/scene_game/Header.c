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

#include "Header.h"
#include "../base/Texture.h"

#include <malloc.h>

struct Header
{
    const int space;
    const int margin;
    const float line_p1_x;
    const float line_p2_x;
    SDL_FRect line_rect;

    SDL_Renderer *renderer;
    SceneGameRect *rect;

    Player currentPlayer;
    Player gameResult;

    Texture *result;
    Texture *player1;
    Texture *player1Icon;
    Texture *player2;
    Texture *player2Icon;
};

void Header_CreateResultText(Header *const this);
void Header_CreatePlayer1Text(Header *const this);
void Header_CreatePlayer2Text(Header *const this);
void Header_SetupResultText(Header *const this);
void Header_SetupPlayer1Text(Header *const this);
void Header_SetupPlayer2Text(Header *const this);

Header *Header_New(SDL_Renderer *renderer, SceneGameRect *rect)
{
    Header *const this = malloc(sizeof (Header));

    *(int *)&this->space = 6;
    *(int *)&this->margin = 20;

    const float w = 134;
    const float x = rect->sidebar_w + ((rect->content_w - w) / 2);
    *(float *)&this->line_p1_x = x - 85.f;
    *(float *)&this->line_p2_x = x + 85.f;
    this->line_rect = (SDL_FRect) {.x = this->line_p1_x, .y = 64.f, .w = w, .h = 4.f};

    this->renderer = renderer;
    this->rect = rect;
    this->currentPlayer = Player_1;
    this->gameResult = None;

    Header_CreateResultText(this);
    Header_CreatePlayer1Text(this);
    Header_CreatePlayer2Text(this);

    return this;
}

void Header_Delete(Header *const this)
{
    if (!this)
        return;

    Texture_Delete(this->result);
    Texture_Delete(this->player1);
    Texture_Delete(this->player1Icon);
    Texture_Delete(this->player2);
    Texture_Delete(this->player2Icon);
    free(this);
}

void Header_ProcessEvent(Header *const this, const SDL_Event *event)
{
    (void)this;(void)event;
}

void Header_Update(Header *const this, double deltaTime)
{
    for (int i = 0; i < 8; ++i)
    {
        if (this->currentPlayer == Player_1 && this->line_rect.x >= this->line_p1_x)
            this->line_rect.x -= 0.1 * deltaTime;
        else if (this->currentPlayer == Player_2 && this->line_rect.x <= this->line_p2_x)
            this->line_rect.x += 0.1 * deltaTime;
    }
}

void Header_Draw(Header *const this)
{
    if (this->gameResult == None)
    {
        SDL_SetRenderDrawColor(this->renderer, 100, 180, 180, 255);
        SDL_RenderFillRectF(this->renderer, &this->line_rect);

        Texture_Draw(this->player1, NULL, NULL);
        Texture_Draw(this->player1Icon, NULL, NULL);
        Texture_Draw(this->player2, NULL, NULL);
        Texture_Draw(this->player2Icon, NULL, NULL);
    }
    else
    {
        int w = 302;
        int x = this->rect->sidebar_w + ((this->rect->content_w - w) / 2);
        SDL_Rect rect = (SDL_Rect) {.x = x, .y = 18, .w = w, .h = 58};

        SDL_SetRenderDrawColor(this->renderer, 120, 200, 200, 255);
        SDL_RenderFillRect(this->renderer, &rect);

        rect.w -= 2; rect.h -= 2; rect.x -= 2; rect.y -= 2;

        SDL_SetRenderDrawColor(this->renderer, 230, 240, 240, 255);
        SDL_RenderFillRect(this->renderer, &rect);

        Texture_Draw(this->result, NULL, NULL);
    }
}

void Header_SetCurrentPlayer(Header *const this, Player currentPlayer, Player gameResult)
{
    this->currentPlayer = currentPlayer;
    this->gameResult = gameResult;

    if (this->gameResult == Player_1)
        Texture_SetText(this->result, "Vitória do jogador 1");
    else if (this->gameResult == Player_2)
        Texture_SetText(this->result, "Vitória do jogador 2");
    else if (this->gameResult == Tied)
        Texture_SetText(this->result, "Deu empate!");

    if (this->gameResult != None)
    {
        Texture_MakeText(this->result);
        Header_SetupResultText(this);
    }
}

void Header_CreateResultText(Header *const this)
{
    this->result = Texture_New(this->renderer);
    Texture_SetupText(this->result, "...", 24, &(SDL_Color) {30, 120, 120, 255});
    Texture_MakeText(this->result);

    Header_SetupResultText(this);
}

void Header_CreatePlayer1Text(Header *const this)
{
    this->player1 = Texture_New(this->renderer);
    Texture_SetupText(this->player1, "Jogador 1", 20, &(SDL_Color) {30, 120, 120, 255});
    Texture_MakeText(this->player1);

    this->player1Icon = Texture_New(this->renderer);
    Texture_LoadImageFromFile(this->player1Icon, "images/player_1.png");

    Header_SetupPlayer1Text(this);
}

void Header_CreatePlayer2Text(Header *const this)
{
    this->player2 = Texture_New(this->renderer);
    Texture_SetupText(this->player2, "Jogador 2", 20, &(SDL_Color) {30, 120, 120, 255});
    Texture_MakeText(this->player2);

    this->player2Icon = Texture_New(this->renderer);
    Texture_LoadImageFromFile(this->player2Icon, "images/player_2.png");

    Header_SetupPlayer2Text(this);
}

void Header_SetupResultText(Header *const this)
{
    int w = Texture_GetWidth(this->result);
    int h = Texture_GetHeight(this->result);

    Texture_SetRect(this->result, &(SDL_Rect) {
                        .x = this->rect->sidebar_w + ((this->rect->content_w - w) / 2),
                        .y = 26,
                        .w = w,
                        .h = h
                    });
}

void Header_SetupPlayer1Text(Header *const this)
{
    int text_w = Texture_GetWidth(this->player1);
    int text_h = Texture_GetHeight(this->player1);
    int icon_w = 28;
    int icon_h = 28;

    int text_x = (this->rect->sidebar_w + ((this->rect->content_w - text_w) / 2)) - (text_w / 2) - this->margin - icon_w - this->space;
    int icon_x = (this->rect->sidebar_w + ((this->rect->content_w - icon_w) / 2)) - (icon_w / 2) - this->margin;

    Texture_SetRect(this->player1, &(SDL_Rect) {
                        .x = text_x,
                        .y = 30,
                        .w = text_w,
                        .h = text_h
                    });

    Texture_SetRect(this->player1Icon, &(SDL_Rect) {
                        .x = icon_x,
                        .y = 32,
                        .w = icon_w,
                        .h = icon_h
                    });
}

void Header_SetupPlayer2Text(Header *const this)
{
    int text_w = Texture_GetWidth(this->player2);
    int text_h = Texture_GetHeight(this->player2);
    int icon_w = 26;
    int icon_h = 26;

    int text_x = (this->rect->sidebar_w + ((this->rect->content_w - text_w) / 2)) + (text_w / 2) + this->margin + icon_w + this->space;
    int icon_x = (this->rect->sidebar_w + ((this->rect->content_w - icon_w) / 2)) + (icon_w / 2) + this->margin;

    Texture_SetRect(this->player2, &(SDL_Rect) {
                        .x = text_x,
                        .y = 30,
                        .w = text_w,
                        .h = text_h
                    });

    Texture_SetRect(this->player2Icon, &(SDL_Rect) {
                        .x = icon_x,
                        .y = 33,
                        .w = icon_w,
                        .h = icon_h
                    });
}
