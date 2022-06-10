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

#include "GameBoard.h"
#include "../../base/Button.h"
#include "../../base/Texture.h"
#include "../../base/Rectangle.h"
#include "../../base/Box.h"
#include "../../base/rect.h"
#include "board_util.h"

#include <malloc.h>

typedef struct GameEvent
{
    GameEventHandler function;
    void *userdata;
} GameEvent;

struct GameBoard
{
    OpenGLRenderer *renderer;
    Rectangle *background;

    Player player;
    Player gameResult;
    int round;

    struct Board
    {
        IRect rect;
        int item_size;
        int space;
        BoardItem items[3][3];
    } board;

    GameEvent gameEvent;

    Texture *player1Texture;
    Texture *player2Texture;

    double p1Angle;
};

void GameBoard_SetupBoard(GameBoard * const self);
void GameBoard_OnItemPress(Button * const button, void *user);
void GameBoard_Check(GameBoard * const self, BoardItem *item);
Player GameBoard_CheckWinner(GameBoard * const self);

GameBoard *GameBoard_New(OpenGLRenderer *renderer, SceneGameRect *sceneGameRect)
{
    GameBoard * const self = malloc(sizeof (GameBoard));

    const int board_size = 304;
    int board_x = sceneGameRect->sidebar_w + ((sceneGameRect->content_w - board_size) / 2);
    int board_y = (sceneGameRect->window_h - board_size) / 2;

    self->board.item_size = 98;
    self->board.space = 5;
    self->board.rect = (IRect) {board_x, board_y, board_size, board_size};

    self->renderer = renderer;
    self->background = Rectangle_New(self->renderer, board_size, board_size);
    self->player = Player_1;
    self->gameResult = None;
    self->round = 0;
    self->gameEvent = (GameEvent) {NULL, NULL};
    self->player1Texture = Texture_New(renderer);
    self->player2Texture = Texture_New(renderer);
    self->p1Angle = 0.0;

    Box_SetPosition(Rectangle_Box(self->background), self->board.rect.x, self->board.rect.y);
    Rectangle_SetColorRGBA(self->background, 80, 160, 160, 255);

    Texture_LoadImageFromFile(self->player1Texture, "images/player_1.png", Nearest);
    Texture_LoadImageFromFile(self->player2Texture, "images/player_2.png", Nearest);

    GameBoard_SetupBoard(self);

    return self;
}

void GameBoard_Delete(GameBoard * const self)
{
    if (!self)
        return;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            Button_Delete(self->board.items[i][j].button);

    Texture_Delete(self->player1Texture);
    Texture_Delete(self->player2Texture);

    free(self);
}

void GameBoard_ProcessEvent(GameBoard * const self, const SDL_Event *event)
{
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            Button_ProcessEvent(self->board.items[row][col].button, event);
}

void GameBoard_Update(GameBoard * const self, double deltaTime)
{
    self->p1Angle = self->p1Angle + 30.0 * deltaTime;

    if (self->p1Angle > 360.0)
        self->p1Angle = 0.0;

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            BoardItem *item = &self->board.items[row][col];
            Texture *icon = Button_Icon(item->button);

            if (icon)
                Texture_SetAngle(icon, item->player == Player_1 ? self->p1Angle : 0);
        }
    }
}

void GameBoard_Draw(GameBoard * const self)
{
    Rectangle_Draw(self->background);

    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            Button_Draw(self->board.items[row][col].button);
}

void GameBoard_SetGameEvent(GameBoard * const self, GameEventHandler callback, void *user)
{
    self->gameEvent.function = callback;
    self->gameEvent.userdata = user;
}

int GameBoard_GetCurrentPlayer(GameBoard * const self)
{
    return self->player;
}

int GameBoard_GetGameResult(GameBoard * const self)
{
    return self->gameResult;
}

void GameBoard_SetupBoard(GameBoard * const self)
{
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            BoardItem *item = &self->board.items[row][col];
            *item = (BoardItem) {.player = 0, .button = Button_New(self->renderer)};

            Box_SetSize(Button_Box(item->button), self->board.item_size, self->board.item_size);
            Box_SetPosition(Button_Box(item->button),
                            self->board.rect.x + (col * self->board.item_size) + (col * self->board.space),
                            self->board.rect.y + (row * self->board.item_size) + (row * self->board.space));

            Button_SetOnPressEvent(item->button, GameBoard_OnItemPress, self);
            Button_SetBackgroundColorRGB(item->button, 210, 240, 240);
            Button_SetBackgroundHoverColorRGB(item->button, 225, 255, 255);
            Button_SetBackgroundPressedColorRGB(item->button, 180, 230, 230);
        }
    }
}

void GameBoard_OnItemPress(Button * const button, void *user)
{
    GameBoard *self = user;

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            BoardItem *item = &self->board.items[row][col];

            if (item->button == button)
            {
                GameBoard_Check(self, item);

                return;
            }
        }
    }
}

void GameBoard_Check(GameBoard * const self, BoardItem *item)
{
    if (item->player != None || self->gameResult != None)
        return;

    item->player = self->player;
    self->gameResult = GameBoard_CheckWinner(self);
    self->player = self->player == Player_1 ? Player_2 : Player_1;

    if (self->gameEvent.function)
        self->gameEvent.function(self, self->gameEvent.userdata);

    Button_SetIcon(item->button, item->player == Player_1 ? self->player1Texture : self->player2Texture);

    self->round++;
}

Player GameBoard_CheckWinner(GameBoard * const self)
{
    Player player = CheckBoardRows(3, self->board.items);

    if (player == None)
    {
        BoardItem items[3][3];
        TransposeBoard(self->board.items, items);
        player = CheckBoardRows(3, items);
    }

    if (player == None)
        player = CheckBoardDiagonals(self->board.items);

    if (player > None)
        return player;

    if (player == None && self->round == 8)
        return Tied;

    return None;
}
