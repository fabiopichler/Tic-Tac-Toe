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

#pragma once

#include "../SceneGameRect.h"

#include <SDL2/SDL.h>

typedef struct GameBoard GameBoard;

typedef void (*GameEventHandler)(GameBoard *const game, void *user);

GameBoard *GameBoard_New(SDL_Renderer *renderer, SceneGameRect *sceneGameRect);
void GameBoard_Delete(GameBoard *const self);
void GameBoard_ProcessEvent(GameBoard *const self, const SDL_Event *event);
void GameBoard_Update(GameBoard *const self, double deltaTime);
void GameBoard_Draw(GameBoard *const self);
void GameBoard_SetGameEvent(GameBoard *const self, GameEventHandler callback, void *user);
int GameBoard_GetCurrentPlayer(GameBoard *const self);
int GameBoard_GetGameResult(GameBoard *const self);
