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
#include "board/GameBoard.h"
#include "../base/Window.h"
#include "../base/Graphics.h"
#include "../base/BasicSceneManager.h"
#include "../base/Button.h"
#include "../base/Texture.h"
#include "../base/Rectangle.h"
#include "board/board_util.h"
#include "Sidebar.h"
#include "Header.h"
#include "Footer.h"

#include "malloc.h"

struct SceneGame
{
    SDL_Renderer *renderer;
    SceneGameRect sceneGameRect;

    int player1WinCount;
    int player2WinCount;
    int tiedCount;

    Rectangle *background;
    GameBoard *gameBoard;
    Sidebar *sidebar;
    Header *header;
    Footer *footer;
};

void SceneGame_NewGame(SceneGame *const self);
void SceneGame_OnPressed(Button *const button, void *user);
void SceneGame_OnGameEvent(GameBoard *const game, void *user);

SceneGame *SceneGame_OnNew(BasicSceneManager *sceneManager)
{
    SceneGame *const self = malloc(sizeof (SceneGame));

    Window *window = BasicSceneManager_Window(sceneManager);
    Graphics *graphics = BasicSceneManager_Graphics(sceneManager);
    SDL_Rect windowRect = Window_GetRect(window);

    self->sceneGameRect.window_w = windowRect.w;
    self->sceneGameRect.window_h = windowRect.h;
    self->sceneGameRect.sidebar_w = 200;
    self->sceneGameRect.sidebar_h = windowRect.h;
    self->sceneGameRect.content_w = windowRect.w - self->sceneGameRect.sidebar_w;
    self->sceneGameRect.content_h = windowRect.h;

    self->renderer = Graphics_GetRenderer(graphics);

    self->player1WinCount = 0;
    self->player2WinCount = 0;
    self->tiedCount = 0;

    self->background = Rectangle_New(self->renderer, self->sceneGameRect.window_w, self->sceneGameRect.window_h);
    self->gameBoard = NULL;
    self->sidebar = Sidebar_New(self->renderer, &self->sceneGameRect);
    self->header = Header_New(self->renderer, &self->sceneGameRect);
    self->footer = Footer_New(self->renderer, &self->sceneGameRect);

    Rectangle_SetColorRGBA(self->background, 190, 225, 225, 255);

    Button *restartButton = Footer_GetRestartButton(self->footer);
    Button_SetOnPressEvent(restartButton, SceneGame_OnPressed, self);

    SceneGame_NewGame(self);

    return self;
}

void SceneGame_OnDelete(SceneGame *const self)
{
    if (!self)
        return;

    GameBoard_Delete(self->gameBoard);
    Footer_Delete(self->footer);
    Header_Delete(self->header);
    Sidebar_Delete(self->sidebar);
    Rectangle_Delete(self->background);

    free(self);
}

void SceneGame_OnProcessEvent(SceneGame *const self, const SDL_Event *event)
{
    Header_ProcessEvent(self->header, event);
    Footer_ProcessEvent(self->footer, event);
    GameBoard_ProcessEvent(self->gameBoard, event);
}

void SceneGame_OnUpdate(SceneGame *const self, double deltaTime)
{
    Header_Update(self->header, deltaTime);
    GameBoard_Update(self->gameBoard, deltaTime);
}

void SceneGame_OnDraw(SceneGame *const self)
{
    Rectangle_Draw(self->background);
    GameBoard_Draw(self->gameBoard);
    Header_Draw(self->header);
    Footer_Draw(self->footer);
    Sidebar_Draw(self->sidebar);
}

void SceneGame_NewGame(SceneGame *const self)
{
    GameBoard_Delete(self->gameBoard);

    self->gameBoard = GameBoard_New(self->renderer, &self->sceneGameRect);

    GameBoard_SetGameEvent(self->gameBoard, SceneGame_OnGameEvent, self);
    Header_SetCurrentPlayer(self->header, Player_1, None);
}

void SceneGame_OnPressed(Button *const button, void *user)
{
    (void)button;
    SceneGame_NewGame(user);
}

void SceneGame_OnGameEvent(GameBoard *const gameBoard, void *user)
{
    SceneGame *const self = user;
    Player player = GameBoard_GetCurrentPlayer(gameBoard);
    Player gameResult = GameBoard_GetGameResult(gameBoard);

    Header_SetCurrentPlayer(self->header, player, gameResult);

    if (gameResult == Player_1)
        Sidebar_SetPlayer1WinText(self->sidebar, ++self->player1WinCount);

    else if (gameResult == Player_2)
        Sidebar_SetPlayer2WinText(self->sidebar, ++self->player2WinCount);

    else if (gameResult == Tied)
        Sidebar_SetTiedCountText(self->sidebar, ++self->tiedCount);
}
