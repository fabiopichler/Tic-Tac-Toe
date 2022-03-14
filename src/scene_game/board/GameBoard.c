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

#include "GameBoard.h"
#include "../../base/Button.h"
#include "../../base/Texture.h"
#include "../../base/Rectangle.h"
#include "../../base/Box.h"
#include "board_util.h"

typedef struct GameEvent
{
    GameEventHandler function;
    void *userdata;
} GameEvent;

struct GameBoard
{
    int item_size;
    int board_space;
    int board_x;
    int board_y;
    SDL_Rect board_rect;

    SDL_Renderer *renderer;
    const SceneGameRect *rect;
    Rectangle *background;

    Player player;
    Player gameResult;
    int round;
    BoardItem board[3][3];

    GameEvent gameEvent;

    Texture *player1Texture;
    Texture *player2Texture;

    double p1Angle;
};

void GameBoard_SetupBoard(GameBoard *const self);
void GameBoard_OnItemPress(Button *const button, void *user);
void GameBoard_Check(GameBoard *const self, BoardItem *item);
Player GameBoard_CheckWinner(GameBoard *const self);

GameBoard *GameBoard_New(SDL_Renderer *renderer, SceneGameRect *rect)
{
    GameBoard *const self = malloc(sizeof (GameBoard));

    const int board_size = 304;

    self->item_size = 98;
    self->board_space = 5;
    self->board_x = rect->sidebar_w + ((rect->content_w - board_size) / 2);
    self->board_y = (rect->window_h - board_size) / 2;
    self->board_rect = (SDL_Rect) {self->board_x, self->board_y, board_size, board_size};

    self->renderer = renderer;
    self->background = Rectangle_New(self->renderer, board_size, board_size);
    self->rect = rect;
    self->player = Player_1;
    self->gameResult = None;
    self->round = 0;
    self->gameEvent = (GameEvent) {NULL, NULL};
    self->player1Texture = Texture_New(renderer);
    self->player2Texture = Texture_New(renderer);
    self->p1Angle = 0.0;

    Box_SetPosition(Rectangle_Box(self->background), self->board_x, self->board_y);
    Rectangle_SetColorRGBA(self->background, 80, 160, 160, 255);

    Texture_LoadImageFromFile(self->player1Texture, "images/player_1.png");
    Texture_LoadImageFromFile(self->player2Texture, "images/player_2.png");

    GameBoard_SetupBoard(self);

    return self;
}

void GameBoard_Delete(GameBoard *const self)
{
    if (!self)
        return;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            Button_Delete(self->board[i][j].button);

    Texture_Delete(self->player1Texture);
    Texture_Delete(self->player2Texture);
    free(self);
}

void GameBoard_ProcessEvent(GameBoard *const self, const SDL_Event *event)
{
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            Button_ProcessEvent(self->board[row][col].button, event);
}

void GameBoard_Update(GameBoard *const self, double deltaTime)
{
    self->p1Angle = self->p1Angle + 30.0 * deltaTime;

    if (self->p1Angle > 360.0)
        self->p1Angle = 0.0;

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            BoardItem *item = &self->board[row][col];
            Texture *icon = Button_Icon(item->button);

            if (icon)
                Texture_SetAngle(icon, item->player == Player_1 ? self->p1Angle : 0);
        }
    }
}

void GameBoard_Draw(GameBoard *const self)
{
    Rectangle_Draw(self->background);

    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            Button_Draw(self->board[row][col].button);
}

void GameBoard_SetGameEvent(GameBoard *const self, GameEventHandler callback, void *user)
{
    self->gameEvent.function = callback;
    self->gameEvent.userdata = user;
}

int GameBoard_GetCurrentPlayer(GameBoard *const self)
{
    return self->player;
}

int GameBoard_GetGameResult(GameBoard *const self)
{
    return self->gameResult;
}

void GameBoard_SetupBoard(GameBoard *const self)
{
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            BoardItem *item = &self->board[row][col];
            *item = (BoardItem) { .player = 0, .button = Button_New(self->renderer) };

            Box_SetSize(Button_Box(item->button), self->item_size, self->item_size);
            Box_SetPosition(Button_Box(item->button),
                            self->board_x + (col * self->item_size) + (col * self->board_space),
                            self->board_y + (row * self->item_size) + (row * self->board_space));

            Button_SetOnPressEvent(item->button, GameBoard_OnItemPress, self);
            Button_SetBackgroundColor(item->button, &(SDL_Color) {210, 240, 240, 255});
            Button_SetBackgroundHoverColor(item->button, &(SDL_Color) {225, 255, 255, 255});
            Button_SetBackgroundPressedColor(item->button, &(SDL_Color) {180, 230, 230, 255});
        }
    }
}

void GameBoard_OnItemPress(Button *const button, void *user)
{
    GameBoard *self = user;

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            BoardItem *item = &self->board[row][col];

            if (item->button == button)
            {
                GameBoard_Check(self, item);
                return;
            }
        }
    }
}

void GameBoard_Check(GameBoard *const self, BoardItem *item)
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

Player GameBoard_CheckWinner(GameBoard *const self)
{
    Player player = CheckBoardRows(3, self->board);

    if (player == None)
    {
        BoardItem board[3][3];
        TransposeBoard(self->board, board);
        player = CheckBoardRows(3, board);
    }

    if (player == None)
        player = CheckBoardDiagonals(self->board);

    if (player > None)
        return player;

    if (player == None && self->round == 8)
        return Tied;

    return None;
}
