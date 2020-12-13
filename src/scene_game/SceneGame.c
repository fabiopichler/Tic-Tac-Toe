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

#include "SceneGame.h"
#include "SceneGameRect.h"
#include "game_board/GameBoard.h"
#include "../base/Button.h"
#include "../base/Texture.h"
#include "game_board/board.h"
#include "Sidebar.h"
#include "Header.h"
#include "Footer.h"

#include "malloc.h"

struct SceneGame
{
    SDL_Renderer *renderer;
    SceneGameRect *rect;

    int player1WinCount;
    int player2WinCount;
    int tiedCount;

    GameBoard *game;
    Sidebar *sidebar;
    Header *header;
    Footer *footer;
};

void SceneGame_NewGame(SceneGame *const this);
void SceneGame_OnPressed(Button *const button, void *user);
void SceneGame_OnGameEvent(GameBoard *const game, void *user);

SceneGame *SceneGame_New(SDL_Renderer *renderer, SDL_Rect windowRect)
{
    SceneGame *const this = malloc(sizeof (SceneGame));

    SceneGameRect *rect = malloc(sizeof (SceneGameRect));
    *(int *)&rect->window_w = windowRect.w;
    *(int *)&rect->window_h = windowRect.h;
    *(int *)&rect->sidebar_w = 200;
    *(int *)&rect->sidebar_h = windowRect.h;
    *(int *)&rect->content_w = windowRect.w - rect->sidebar_w;
    *(int *)&rect->content_h = windowRect.h;

    this->renderer = renderer;
    this->rect = rect;
    this->game = NULL;
    this->player1WinCount = 0;
    this->player2WinCount = 0;
    this->tiedCount = 0;
    this->sidebar = Sidebar_New(this->renderer, this->rect);
    this->header = Header_New(this->renderer, this->rect);
    this->footer = Footer_New(this->renderer, this->rect);

    Button *restartButton = Footer_GeRestartButton(this->footer);
    Button_SetOnPressEvent(restartButton, SceneGame_OnPressed, this);

    SceneGame_NewGame(this);

    return this;
}

void SceneGame_Delete(SceneGame *const this)
{
    if (!this)
        return;

    Game_Delete(this->game);
    Footer_Delete(this->footer);
    Header_Delete(this->header);
    Sidebar_Delete(this->sidebar);
    free(this->rect);
    free(this);
}

void SceneGame_ProcessEvent(SceneGame *const this, const SDL_Event *event)
{
    Header_ProcessEvent(this->header, event);
    Footer_ProcessEvent(this->footer, event);
    Game_ProcessEvent(this->game, event);
}

void SceneGame_Update(SceneGame *const this, double deltaTime)
{
    Header_Update(this->header, deltaTime);
    GameBoard_Update(this->game, deltaTime);
}

void SceneGame_Draw(SceneGame *const this)
{
    Game_Draw(this->game);
    Header_Draw(this->header);
    Footer_Draw(this->footer);
    Sidebar_Draw(this->sidebar);
}

void SceneGame_NewGame(SceneGame *const this)
{
    Game_Delete(this->game);

    this->game = Game_New(this->renderer, this->rect);
    Game_SetGameEvent(this->game, SceneGame_OnGameEvent, this);
    Header_SetCurrentPlayer(this->header, Player_1, None);
}

void SceneGame_OnPressed(Button *const button, void *user)
{
    (void)button;
    SceneGame_NewGame(user);
}

void SceneGame_OnGameEvent(GameBoard *const game, void *user)
{
    SceneGame *const this = user;
    Player player = Game_GetCurrentPlayer(game);
    Player gameResult = Game_GetGameResult(game);

    Header_SetCurrentPlayer(this->header, player, gameResult);

    if (gameResult == Player_1)
        Sidebar_SetPlayer1WinText(this->sidebar, ++this->player1WinCount);
    else if (gameResult == Player_2)
        Sidebar_SetPlayer2WinText(this->sidebar, ++this->player2WinCount);
    else if (gameResult == Tied)
        Sidebar_SetTiedCountText(this->sidebar, ++this->tiedCount);
}
